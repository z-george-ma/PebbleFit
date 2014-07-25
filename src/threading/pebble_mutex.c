#include <pebble.h>
#include "mutex.h"

void pebble_mutex_acquire(volatile unsigned char* lock)
{
  while(!mutex_acquire(lock))
    psleep(0); // or sleep 1?
}
 
 
void pebble_mutex_release(volatile unsigned char* lock)
{
  while(!mutex_release(lock))
    psleep(0); // or sleep 1?
}
