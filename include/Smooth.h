#ifndef Smooth_h
#define Smooth_h

#include <Arduino.h>


class Smooth
{
    // Double Exponential Smoothing
    // https://www.itl.nist.gov/div898/handbook/pmc/section4/pmc433.htm
  public:
    Smooth(uint8_t pin, float alpha = 0.1, float gamma = 0.5)
    {
      _pin = pin;
      _alpha = alpha;
      _gamma = gamma;
    }
    uint16_t read()
    {
      _raw_value = analogRead(_pin);
      _smoothed = _alpha * _raw_value + (1. - _alpha) * (_smoothed + _beta);
      _beta = _gamma * (_smoothed - _old_smoothed) + (1. - _gamma) * _beta;
      _old_smoothed = _smoothed;
      // smoothed value can overshoot and become negative, overflowing our unsigned int
      return max(0, _smoothed);
    }

  private:
    uint8_t _pin;
    float _alpha;
    float _gamma;
    int16_t _beta = 0;
    uint16_t _raw_value = 0;
    int16_t _smoothed = 0;
    int16_t _old_smoothed = 0;
};
#endif