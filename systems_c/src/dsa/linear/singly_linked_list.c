#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
  int data;
  struct Node *next;
} Node;

Node *createNode(int data) {
  Node *newNode = malloc(sizeof(Node));
  if (newNode == NULL) {
    printf("Failed to allocate memory.\n");
    exit(1);
  }
  newNode->data = data;
  newNode->next = NULL;
  return newNode;
}

void insertEnd(Node **head, int data) {
  Node *newNode = createNode(data);

  if (*head == NULL) {
    *head = newNode;
    return;
  }

  Node *temp = *head;
  while (temp->next != NULL) {
    temp = temp->next;
  }

  temp->next = newNode;
}

void insertFirst(Node **head, int data) {
  Node *newNode = createNode(data);

  newNode->next = *head;
  *head = newNode;
}

void deleteFirstNode(Node **head) {
  if (*head == NULL) {
    return;
  }
  Node *temp = *head;
  *head = (*head)->next;
  free(temp);
}

void deleteLastNode(Node **head) {
  if (*head == NULL) {
    return;
  }
  if ((*head)->next == NULL) {
    free(*head);
    *head = NULL;
    return;
  }
  Node *temp = *head;

  while (temp->next->next != NULL) {
    temp = temp->next;
  }
  free(temp->next);
  temp->next = NULL;
}

int search(Node *head, int key) {
  Node *temp = head;

  while (temp != NULL) {
    if (temp->data == key) {
      return 1;
    }
    temp = temp->next;
  }
  return 0;
}

void insertAt(Node **head, int data, int index) {
  if (index < 0) {
    return;
  }
  if (index == 0) {
    insertFirst(head, data);
    return;
  }

  Node *temp = *head;

  for (int i = 0; i < index - 1 && temp != NULL; i++) {
    temp = temp->next;
  }
  if (temp == NULL) {
    return;
  }
  Node *newNode = createNode(data);
  newNode->next = temp->next;
  temp->next = newNode;
}

void deleteAt(Node **head, int index) {
  if (index < 0 || *head == NULL)
    return;

  if (index == 0) {
    deleteFirstNode(head);
  }

  Node *temp = *head;
  for (int i = 0; i < index - 1 && temp != NULL; i++) {
    temp = temp->next;
  }

  if (temp == NULL || temp->next == NULL) {
    return;
  }

  Node *toDelete = temp->next;
  temp->next = toDelete->next;
  free(toDelete);
}

int len(Node *head) {
  int count = 0;
  Node *temp = head;
  while (temp != NULL) {
    count++;
    temp = temp->next;
  }
  return count;
}

void display(Node *head) {
  Node *temp = head;
  while (temp != NULL) {
    printf("%d ", temp->data);
    temp = temp->next;
  }
  printf("NULL\n");
}

void freeList(Node *head) {
  Node *temp;
  while (head != NULL) {
    temp = head;
    head = head->next;
    free(temp);
  }
}

int main(void) {
  Node *head = NULL;

  insertEnd(&head, 10);
  insertEnd(&head, 20);
  insertEnd(&head, 30);
  insertEnd(&head, 40);
  insertEnd(&head, 50);
  insertFirst(&head, 100);
  display(head);

  deleteFirstNode(&head);
  printf("After removing first node : \n");
  display(head);

  deleteLastNode(&head);
  printf("After removing last node : \n");
  display(head);

  if (search(head, 20)) {
    printf("Found\n");
  } else {
    printf("Not found\n");
  }

  insertAt(&head, 999, 3);
  display(head);

  deleteAt(&head, 3);
  display(head);

  printf("length : %d\n", len(head));

  freeList(head);

  return 0;
}
