#ifndef __PEBBLE_QUEUE__H__
#define __PEBBLE_QUEUE__H__

#include <stdbool.h>

typedef struct Queue Queue;

Queue *queue_create(unsigned int buffer_size);

void queue_destroy(Queue *q);

bool queue_enqueue(Queue *q, void *data, unsigned int size);

bool queue_dequeue(Queue *q, void *buf, unsigned int *size);

#endif
