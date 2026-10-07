#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 8080
#define MAX_EVENTS 64
#define BUFFER_SIZE 1024

// Set a file descriptor to non-blocking mode using fcntl
static int set_nonblocking(int fd) {
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1) {
    perror("fcntl F_GETFL");
    return -1;
  }
  if (fcntl(fd, F_SETFL, flags | O_NONBLOCK) == -1) {
    perror("fcntl F_SETFL O_NONBLOCK");
    return -1;
  }
  return 0;
}

int main(void) {
  int listen_fd, epoll_fd;
  struct sockaddr_in server_addr;
  struct epoll_event ev, events[MAX_EVENTS];

  // 1. Create stream socket (TCP)
  listen_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_fd < 0) {
    perror("socket");
    exit(EXIT_FAILURE);
  }

  // 2. Allow port reuse immediately after restart (prevents EADDRINUSE in
  // TIME_WAIT)
  int opt = 1;
  if (setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("setsockopt SO_REUSEADDR");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 3. Make listening socket non-blocking
  if (set_nonblocking(listen_fd) < 0) {
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 4. Bind socket to 0.0.0.0:8080
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("bind");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 5. Mark socket as passive listener
  if (listen(listen_fd, SOMAXCONN) < 0) {
    perror("listen");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 6. Create epoll instance in kernel
  epoll_fd = epoll_create1(0);
  if (epoll_fd < 0) {
    perror("epoll_create1");
    close(listen_fd);
    exit(EXIT_FAILURE);
  }

  // 7. Add listening socket to epoll monitoring set for read events (EPOLLIN)
  // EPOLLET = Edge-Triggered mode (notifies only on new state changes)
  ev.events = EPOLLIN | EPOLLET;
  ev.data.fd = listen_fd;
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, listen_fd, &ev) < 0) {
    perror("epoll_ctl listen_fd");
    close(listen_fd);
    close(epoll_fd);
    exit(EXIT_FAILURE);
  }

  printf("Non-blocking epoll TCP server listening on port %d...\n", PORT);

  // 8. Main event loop
  while (1) {
    int nfds = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
    if (nfds < 0) {
      if (errno == EINTR)
        continue; // Interrupted by system signal
      perror("epoll_wait");
      break;
    }

    for (int i = 0; i < nfds; i++) {
      int current_fd = events[i].data.fd;

      // CASE A: Incoming connection on listening socket
      if (current_fd == listen_fd) {
        // In Edge-Triggered mode, accept in a loop until EAGAIN/EWOULDBLOCK
        while (1) {
          struct sockaddr_in client_addr;
          socklen_t client_len = sizeof(client_addr);
          int client_fd =
              accept(listen_fd, (struct sockaddr *)&client_addr, &client_len);

          if (client_fd < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // Drained all pending connections in queue
              break;
            }
            perror("accept");
            break;
          }

          // Set client socket to non-blocking
          if (set_nonblocking(client_fd) < 0) {
            close(client_fd);
            continue;
          }

          // Add client socket to epoll watchlist (EPOLLIN | EPOLLET)
          ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
          ev.data.fd = client_fd;
          if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, client_fd, &ev) < 0) {
            perror("epoll_ctl client_fd");
            close(client_fd);
            continue;
          }

          char client_ip[INET_ADDRSTRLEN];
          inet_ntop(AF_INET, &client_addr.sin_addr, client_ip,
                    sizeof(client_ip));
          printf("[+] Client connected [fd=%d] from %s:%d\n", client_fd,
                 client_ip, ntohs(client_addr.sin_port));
        }
      }
      // CASE B: Data or disconnection event on existing client socket
      else {
        // Check if client disconnected or hit error
        if (events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR)) {
          printf("[-] Client disconnected [fd=%d]\n", current_fd);
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, current_fd, NULL);
          close(current_fd);
          continue;
        }

        // Read incoming data in a loop until Rx buffer is empty (EAGAIN)
        char buffer[BUFFER_SIZE];
        int client_disconnected = 0;

        while (1) {
          ssize_t bytes_read = recv(current_fd, buffer, sizeof(buffer) - 1, 0);

          if (bytes_read < 0) {
            if (errno == EAGAIN || errno == EWOULDBLOCK) {
              // Drained all data from kernel Rx buffer
              break;
            }
            perror("recv error");
            client_disconnected = 1;
            break;
          } else if (bytes_read == 0) {
            // EOF: Peer closed connection
            client_disconnected = 1;
            break;
          }

          buffer[bytes_read] = '\0';
          printf("[fd=%d] Received: %s", current_fd, buffer);

          // Echo data back to client (handling partial sends)
          ssize_t total_sent = 0;
          while (total_sent < bytes_read) {
            ssize_t bytes_sent = send(current_fd, buffer + total_sent,
                                      bytes_read - total_sent, 0);
            if (bytes_sent < 0) {
              if (errno == EAGAIN || errno == EWOULDBLOCK) {
                // Tx buffer full — in production, register EPOLLOUT here
                break;
              }
              perror("send error");
              client_disconnected = 1;
              break;
            }
            total_sent += bytes_sent;
          }
        }

        if (client_disconnected) {
          printf("[-] Closing connection [fd=%d]\n", current_fd);
          epoll_ctl(epoll_fd, EPOLL_CTL_DEL, current_fd, NULL);
          close(current_fd);
        }
      }
    }
  }

  close(listen_fd);
  close(epoll_fd);
  return 0;
}
