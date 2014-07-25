#ifndef __PEBBLE_FIT_FILTERS_H__
#define __PEBBLE_FIT_FILTERS_H__

#include <stdint.h>

int32_t iir_5_35_filter(int32_t input);

void zero_crossing_set_detection_threshold(uint32_t threshold);
uint32_t zero_crossing_detect(int16_t x, int16_t y, int16_t z, uint64_t timestamp, bool did_vibrate);
#endif
