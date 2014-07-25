#ifndef __PEBBLE_TASK_H__
#define __PEBBLE_TASK_H__

typedef struct Task Task;
typedef struct Promise Promise;

typedef void (*TaskHandler)(Promise *promise);

Task *task_create(unsigned int initial_handlers_size);
void task_reset(Task *task);
void task_set_default_handler(Task *task, TaskHandler handler);
void task_on_complete(Task *task, TaskHandler handler);
void task_destroy(Promise *promise);

Promise *task_create_promise(Task *task, unsigned int default_buffer_size);

void *promise_get_value(Promise *promise);
unsigned int promise_get_value_size(Promise *promise);
void promise_complete(Promise *promise, void *p_data, unsigned int size);
#endif
