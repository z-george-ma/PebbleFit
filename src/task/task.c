#include "../data/array_list.h"
#include <stdlib.h>
#include <string.h>

typedef struct Task {
  ArrayList *handlers;
  TaskHandler default_handler;
  Promise *promise;
  unsigned int handler_pos;
} Task;

typedef struct Promise {
  Task *task;
  void *buf;
  unsigned int buf_size;
  unsigned int size;
} Promise;

void task_destroy(Promise *promise) {
  array_list_free(promise->task->handlers);
  free(promise->task);
  free(promise->buf);
  free(promise);
}

Task *task_create(unsigned int initial_handlers_size) {
  Task *task = (Task *)malloc(sizeof(Task));
  task->handlers = array_list_create(sizeof(TaskHandler), initial_handlers_size);
  task->handler_pos = 0;
  task->default_handler = task_destroy;
  task->promise = NULL;
  return task;
}

void task_reset(Task *task) {
  task->handler_pos = 0;
}

Promise *task_create_promise(Task *task, unsigned int default_buffer_size) {
  Promise *promise = task->promise;
  if(promise != NULL) {
    if (promise->buf_size < default_buffer_size) {
      free(promise->buf);
      free(promise);
      promise = NULL;
    }
  }
  
  if (promise == NULL) {
    promise = (Promise *)malloc(sizeof(Promise));
    promise->task = task;
    promise->buf_size = default_buffer_size;
    promise->buf = malloc(default_buffer_size);
  }

  return promise;
}

void task_set_default_handler(Task *task, TaskHandler handler) {
  task->default_handler = handler;
}

void task_on_complete(Task *task, TaskHandler handler) {
  array_list_add(task->handlers, handler);
}

void task_call(Task *task, Promise *promise) {
  TaskHandler handler = (TaskHandler)array_list_get(task->handlers, task->handler_pos++);
  handler(promise);
}

void *promise_get_value(Promise *promise) {
  return promise->buf;
}

unsigned int promise_get_value_size(Promise *promise) {
  return promise->size;
}

void promise_complete(Promise *promise, void *p_data, unsigned int size) {
  if (promise->buf_size < size) {
    free(promise->buf);
    promise->buf = malloc(size);
    promise->buf_size = size;
  }

  promise->size = size;
  memcpy(promise->buf, p_data, size);

  task_call(promise->task, promise);
}

