#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *data;
  int size;
  int capacity;
} DynamicArray;

void initArray(DynamicArray *a, int initial_capacity) {
  a->data = malloc(initial_capacity * sizeof(int));
  a->size = 0;
  a->capacity = initial_capacity;
}

void insertArray(DynamicArray *a, int element) {
  if (a->size == a->capacity) {
    a->capacity *= 2;

    int *temp = (int *)realloc(a->data, a->capacity * sizeof(int));
    if (temp == NULL) {
      printf("Resizing failed.\n");
      exit(EXIT_FAILURE);
    }
    a->data = temp;
  }
  a->data[a->size] = element;
  a->size++;
}

void freeArray(DynamicArray *a) {
  free(a->data);
  a->data = NULL;
  a->size = 0;
  a->capacity = 0;
}

int main(int argc, char *argv[]) {
  printf("Dynamic array.\n");
  DynamicArray arr;
  initArray(&arr, 2);

  for (int i = 1; i <= 5; i++) {
    insertArray(&arr, i * 100);
  }

  for (int i = 0; i < arr.size; i++) {
    printf("%d ", arr.data[i]);
  }
  printf("\nCapacity: %d, Size: %d\n", arr.capacity, arr.size);

  freeArray(&arr);
  return EXIT_SUCCESS;
}
