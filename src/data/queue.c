#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

typedef struct Queue {
  void *head;
  void *tail;
  unsigned int size;
} Queue;

static char *unwind_read(char *dest, char *pos, unsigned int size, char *src_head, unsigned int src_length) {
  unsigned int remaining_bytes = src_head + src_length - pos;
  if (remaining_bytes > size) {
    memcpy(dest, pos, size);
    return pos + size;
  }
  else if (remaining_bytes == size){
    memcpy(dest, pos, size);
    return src_head;
  }
  else {
    unsigned int copied_bytes = size - remaining_bytes;
    memcpy(dest, pos, remaining_bytes);
    memcpy(dest + remaining_bytes, src_head, copied_bytes);
    return src_head + copied_bytes;
  }
}

static char *unwind_write(char *pos, char *src, unsigned int size, char *dest_head, unsigned int dest_length) {
  unsigned int remaining_bytes = dest_head + dest_length - pos;
  if (remaining_bytes > size) {
    memcpy(pos, src, size);
    return pos + size;
  }
  else if (remaining_bytes == size){
    memcpy(pos, src, size);
    return dest_head;
  }
  else {
    unsigned int copied_bytes = size - remaining_bytes;
    memcpy(pos, src, remaining_bytes);
    memcpy(dest_head, src + remaining_bytes, copied_bytes);
    return dest_head + copied_bytes;
  }
}

Queue *queue_create(unsigned int buffer_size) {
  Queue *queue = (Queue *)malloc(sizeof(Queue) + buffer_size);
  queue->head = queue->tail = queue + 1;
  queue->size = buffer_size;
  return queue;
}

void queue_destroy(Queue *q) {
  free(q);
}

bool queue_enqueue(Queue *q, void *data, unsigned int size) {
  char *adjusted_head = q->head > q->tail ? (char *)q->head : (char *)q->head +q->size;
  
  if(adjusted_head - (char *)q->tail < (int)(size + sizeof(size)))
    return false;
 
  q->tail = unwind_write(q->tail, (char *)&size, sizeof(size), (char *)(q + 1), q->size); 
  q->tail = unwind_write(q->tail, (char *)data, size, (char *)(q + 1), q->size); 
  
  return true;
}

bool queue_dequeue(Queue *q, void *buf, unsigned int *size) {
  if (q->head == q->tail) {
    *size = 0;
    return false;
  }

  unsigned int buf_size = *size;

  char *head = unwind_read((char *)size, q->head, sizeof(size), (char *)(q + 1), q->size);

  if (*size > buf_size)
    return false;

  q->head = unwind_read((char *)buf, head, *size, (char *)(q + 1), q->size);

  return true;
}
