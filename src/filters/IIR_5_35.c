#include <stdint.h>
#include <stdbool.h>
#define NTAP 4

static int32_t _A0 = 13251, _B0 = 32768;
static int32_t _ACoef[NTAP] = {
  13251,
  0,
  -26502,
  0
};

static int32_t _BCoef[NTAP] = {
  6416
  -380,
  -4625,
  -28088
};

static int32_t _x[NTAP], _y[NTAP];
static int _pos = 0;
static bool _feeding = true;

int32_t iir_5_35_filter(int32_t input) {
  int32_t output;
  if (_feeding) {
    output = input * _A0;
  }
  else {
    int i, coef_pos = 0;
    output = input * _A0;
    for(i = _pos; i < NTAP; i++) {
      output += _x[i] * _ACoef[coef_pos] - _y[i] * _BCoef[coef_pos];
      coef_pos++;
    }
    for(i = 0; i < _pos; i++) {
      output += _x[i] * _ACoef[coef_pos] - _y[i] * _BCoef[coef_pos];
      coef_pos++;
    }
  }

  _x[_pos] = input;
  _y[_pos] = output = output / _B0;

  if (++_pos >= NTAP - 1) {
    _pos = 0;
    _feeding = false;
  }
  
  return output;
}
#undef NTAP
