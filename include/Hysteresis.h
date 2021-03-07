#ifndef Hysteresis_h
#define Hysteresis_h

#include <Arduino.h>


template <typename T>
class Hysteresis : public Printable
{
  public:
    bool state = false;
    T threshold;

    Hysteresis(T threshold, T deadband) : threshold(threshold), _deadband(deadband) {}
    bool update(const T value)
    {
      if (state) {
        if (value <= threshold - _deadband)
          state = false;
      } else {
        if (value >= threshold + _deadband)
          state = true;
      }
      return state;
    }
    void set_threshold(T threshold_)
    {
      threshold = threshold_;
    }
    size_t printTo(Print& p) const {
      size_t r = 0;
      r += p.print("<Threshold: ");
      r += p.print(threshold);
      r += p.print("> State: ");
      r += p.print(state);
      return r;
    }

  private:
    T _deadband;
};


template <typename T>
class ThreeWayHysteresis : public Printable
{
  public:
    bool high = false;
    bool low = false;
    ThreeWayHysteresis(T low_threshold, T high_threshold, T deadband) : _high(high_threshold, deadband), _low(low_threshold, deadband) {}
    void update(const T value)
    {
      low = _low.update(value);
      high = _high.update(value);
    }
    size_t printTo(Print& p) const {
      size_t r = 0;
      r += p.print("<Low: ");
      r += p.print(_low.threshold);
      r += p.print(" High: ");
      r += p.print(_high.threshold);
      r += p.print("> State: ");
      r += p.print(low);
      r += p.print(" ");
      r += p.print(high);
      return r;
    }

  private:
    Hysteresis<T> _high;
    Hysteresis<T> _low;
};


template <typename T>
class FiveWayHysteresis : public Printable
{
  public:
    bool highest = false;
    bool high = false;
    bool low = false;
    bool lowest = false;
    // Constructor with 4 thresholds and a deadband
    FiveWayHysteresis(T lowest, T low, T high, T highest, T deadband) : _lowest(lowest, deadband),
                                                                        _low(low, deadband),
                                                                        _high(high, deadband),
                                                                        _highest(highest, deadband) {}
    // Constructor with 5 targets and a deadband
    FiveWayHysteresis(T lowest, T low, T mid, T high, T highest, T deadband) : _lowest((lowest + low) / 2, deadband),
                                                                               _low((low + mid) / 2, deadband),
                                                                               _high((mid + high) / 2, deadband),
                                                                               _highest((high + highest) / 2, deadband) {}
    void update(const T value)
    {
      highest = _highest.update(value);
      high = _high.update(value);
      low = _low.update(value);
      lowest = _lowest.update(value);
    }
    size_t printTo(Print& p) const {
      size_t r = 0;
      r += p.print("<Lowest: ");
      r += p.print(_lowest.threshold);
      r += p.print(" Low: ");
      r += p.print(_low.threshold);
      r += p.print(" High: ");
      r += p.print(_high.threshold);
      r += p.print(" Highest: ");
      r += p.print(_highest.threshold);
      r += p.print("> State: ");
      r += p.print(lowest);
      r += p.print(" ");
      r += p.print(low);
      r += p.print(" ");
      r += p.print(high);
      r += p.print(" ");
      r += p.print(highest);
      return r;
    }

  protected:
    Hysteresis<T> _lowest;
    Hysteresis<T> _low;
    Hysteresis<T> _high;
    Hysteresis<T> _highest;
};


template <typename T>
class AutoFiveWayHysteresis : public FiveWayHysteresis<T>
{
  public:
    AutoFiveWayHysteresis(T default_, T deadband) : FiveWayHysteresis<T>(default_, default_, default_, default_, deadband)
    {
      _min = default_;
      _max = default_;
    }
    void update_thresholds(const T value)
    {
      _min = min(_min, value);
      _max = max(_max, value);
      T interval = (_max - _min) / 10;
      FiveWayHysteresis<T>::_lowest.set_threshold(_min + interval);
      FiveWayHysteresis<T>::_low.set_threshold(_min + 3 * interval);  // Should be tweaked to 3.5 according to my shifter
      FiveWayHysteresis<T>::_high.set_threshold(_min + 7 * interval); // Should be tweaked to 6 according to my shifter
      FiveWayHysteresis<T>::_highest.set_threshold(_min + 9 * interval);
    }
    void update(const T value)
    {
      if (value > _max || value < _min) update_thresholds(value);
      FiveWayHysteresis<T>::update(value);
    }

  private:
    T _min;
    T _max;
};
#endif