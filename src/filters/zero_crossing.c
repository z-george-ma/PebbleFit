#include "../math/math.h"
#include <stdbool.h>

#define BUFFER_LENGTH 20
static uint32_t _buf[BUFFER_LENGTH], *_head = _buf;

static uint32_t _sum = 0;
static int8_t _accel_flag = 0;
static uint32_t _threshold = 50;
static bool _feeding = true;

void zero_crossing_set_detection_threshold(uint32_t threshold) {
  _threshold = threshold;
}

uint32_t zero_crossing_detect(int16_t x, int16_t y, int16_t z, uint64_t timestamp, bool did_vibrate) {
  uint32_t scale, intensity = 0;
  bool sign;

  if (did_vibrate)
    scale = (_head == _buf) ? (_feeding ? 1000 : _buf[BUFFER_LENGTH-1]) : *(_head-1);
  else
    scale = isqrt(x * x + y * y + z * z);

  if (_feeding)
    _sum += scale;
  else
    _sum += (scale - *_head);

  *(_head++) = scale;
  
  if (_head - _buf >= BUFFER_LENGTH) {
    _head = _buf;
    _feeding = false;
  }
  
  uint32_t avg = _sum / BUFFER_LENGTH;
  sign = scale > avg;
  intensity = sign ? scale - avg : avg - scale;

  uint32_t output = 0;

  if (intensity > _threshold) {
    if (sign ? _accel_flag < 0 : _accel_flag > 0) {
      output = intensity;
    }
    _accel_flag = sign ? 1 : -1;
  }
  else
    _accel_flag = 0;
  
  return output;
}

#undef BUFFER_LENGTH
