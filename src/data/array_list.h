#ifndef __PEBBLE_ARRAY_LIST_H__
#define __PEBBLE_ARRAY_LIST_H__

typedef struct ArrayList ArrayList;

ArrayList *array_list_create(unsigned int type_size, unsigned int initial_size);
void array_list_add(ArrayList *list, void *ptr);
void *array_list_get(ArrayList *list, unsigned int i);
void array_list_set(ArrayList *list, unsigned int i, void *ptr);
void array_list_free(ArrayList *list);

#endif
