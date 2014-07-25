#ifndef __PEBBLE_MUTEX_H__
#define __PEBBLE_MUTEX_H__

bool mutex_acquire(volatile unsigned char* lock);
bool mutex_release(volatile unsigned char* lock);

void pebble_mutex_acquire(volatile unsigned char* lock);
void pebble_mutex_release(volatile unsigned char* lock);
#endif
