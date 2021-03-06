#ifndef Smooth_h
#define Smooth_h

#include <Arduino.h>


class Smooth
{
    // Double Exponential Smoothing
    // https://www.itl.nist.gov/div898/handbook/pmc/section4/pmc433.htm
  public:
    Smooth(uint8_t pin, float new_weight = 0.1, float speed_weight = 0.5)
    {
      _pin = pin;
      _new_weight = new_weight;
      _speed_weight = speed_weight;
    }
    uint16_t read()
    {
      _raw_value = analogRead(_pin);
      _smoothed = _new_weight * _raw_value + (1 - _new_weight) * (_smoothed + _diff_speed);
      _diff_speed = _speed_weight * (_smoothed - _old_value) + (1 - _speed_weight) * _diff_speed;
      _old_value = _smoothed;
      return _smoothed;
    }

  private:
    uint8_t _pin;
    float _new_weight;
    float _speed_weight;
    uint16_t _raw_value = 2048;
    uint16_t _smoothed = 2048;
    uint16_t _diff_speed = 0;
    uint16_t _old_value = 2048;
};
#endif