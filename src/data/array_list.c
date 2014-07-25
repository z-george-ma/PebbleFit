#include <stdlib.h>
#include <string.h>

typedef struct ArrayList ArrayList;

struct ArrayList {
  unsigned int type_size;
  unsigned int position;
  unsigned int blk_length;
  ArrayList *next;
} __attribute__((packed));

ArrayList *array_list_create(unsigned int type_size, unsigned int initial_size) {
  ArrayList *list = (ArrayList *)malloc(initial_size * type_size + sizeof(ArrayList));
  list->type_size = type_size;
  list->position = 0;
  list->blk_length = initial_size;
  list->next = NULL;
  return list;
}

void array_list_add(ArrayList *list, void *ptr) {
  ArrayList *tail = list;
  while(tail->next != NULL)
    tail = tail->next;

  if (tail->position == tail->blk_length) {
    tail = (tail->next = array_list_create(tail->type_size, tail->blk_length));
  }

  memcpy((char *)tail + sizeof(ArrayList) + tail->type_size * tail->position, ptr, tail->type_size);
  tail->position++;
}

void *array_list_get(ArrayList *list, unsigned int i) {
  int hop = i / list->blk_length;
  unsigned int remainder = i % list->blk_length;
  ArrayList *blk = list;
  for(int j=0; j<hop; j++) {
    blk = blk->next;
    if (blk == NULL)
      return NULL;
  }

  if (blk->position <= remainder)
    return NULL;

  return (void *)((char *)blk + sizeof(ArrayList) + blk->type_size * remainder);
}

void array_list_set(ArrayList *list, unsigned int i, void *ptr) {
  int hop = i / list->blk_length;
  unsigned int remainder = i % list->blk_length;
  ArrayList *blk = list;
  for(int j=0; j<hop; j++) {
    blk = blk->next;
    if (blk == NULL)
      return;
  }

  if (blk->position <= remainder)
    return;

  memcpy(((char *)blk + sizeof(ArrayList) + blk->type_size * remainder), ptr, blk->type_size);
}

void array_list_free(ArrayList *list) {
  ArrayList *tail = list->next, *temp;
  while(tail != NULL) {
    temp = tail;
    tail = tail->next;
    free(temp);
  }

  free(list);
}

