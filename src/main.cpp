#include <Arduino.h>
#include "Smooth.h"
#include "Hysteresis.h"

//#define DEBUG


template <typename T>
class HShifter : public Printable
{
  public:
    int8_t gear = 0;
    HShifter(T reverse, T first, T third, T fifth, T seventh, T x_deadband, T down, T up, T y_deadband) :
       _HX(seventh, fifth, third, first, reverse, x_deadband), _HY(down, up, y_deadband) {}
    int8_t update(T x, T y)
    {
      _HX.update(x);
      _HY.update(y);
      if (_HY.low && !_HY.high) {
        gear = 0; // Neutral
      } else if (!_HY.low) {
        if (_HX.highest)
          gear = -2;  // not possible
        else if (_HX.high)
          gear = 2;
        else if (_HX.low)
          gear = 4;
        else if (_HX.lowest)
          gear = 6;
        else
          gear = -2;  // not possible
      } else {
        if (_HX.highest)  // X-axis is reversed
          gear = -1;
        else if (_HX.high)
          gear = 1;
        else if (_HX.low)
          gear = 3;
        else if (_HX.lowest)
          gear = 5;
        else
          gear = 7;
      }
      return gear;
    }
    size_t printTo(Print& p) const {
      size_t r = 0;
      r += p.print(" X : ");
      r += p.print(_HX);
      r += p.print(" Y : ");
      r += p.print(_HY);
      r += p.print(" gear : ");
      r += p.print(gear);
      return r;
    }

  private:
    FiveWayHysteresis<T> _HX;
    ThreeWayHysteresis<T> _HY;
};


//Smooth SmoothX = Smooth(A1);
//Smooth SmoothY = Smooth(A2);

// Tune threshold and deadband numbers if not using 11 bits ADC
Hysteresis<uint16_t> SeqX = Hysteresis<uint16_t>(1024, 128);
Hysteresis<uint16_t> SeqY = Hysteresis<uint16_t>(1024, 128);
// Average X value for gear :              R    1st   3rd  5th  7th
HShifter<uint16_t> H = HShifter<uint16_t>(1495, 1165, 775, 425, 135, 32,
                                          512, 1536, 256);

// Used to remember previous values and update Joystick buttons only when necessary
bool old_down = false;
bool old_up = false;
int8_t old_gear = 0;


void setup() {
#ifdef DEBUG
  Serial.begin(115200);
#endif
  pinMode(14, INPUT);
  analogReadResolution(11); // 12 bits ADC is very jittery and offers no extra info over 11 bits
  Joystick.useManualSend(true);
  // Center everything
  Joystick.hat(-1);
  Joystick.X(512);
  Joystick.Y(512);
  Joystick.Z(512);
  Joystick.Zrotate(512);
  Joystick.sliderLeft(512);
  Joystick.sliderRight(512);
  Joystick.send_now();
}


void loop() {
  const bool seq = digitalRead(14);
  //const uint16_t x = SmoothX.read();
  //const uint16_t y = SmoothY.read();
  const uint16_t x = analogRead(A1);
  const uint16_t y = analogRead(A2);

  if (seq == true) {
    // Seq shifts are active low
    const bool down = !SeqX.update(x);
    const bool up = !SeqY.update(y);
    if (down != old_down || up != old_up) {
      Joystick.button(9, down);
      old_down = down;
      Joystick.button(10, up);
      old_up = up;
      Joystick.send_now();
    }
  } else {
    const int8_t gear = H.update(x, y);
    if (old_gear != gear) {
      if (gear > 0) Joystick.button(gear, true); // Not neutral or reverse
      if (gear == -1) Joystick.button(8, true); // Reverse
      if (old_gear > 0) Joystick.button(old_gear, false);
      if (old_gear == -1) Joystick.button(8, false);
      old_gear = gear;
      Joystick.send_now();
    }
  }

#ifdef DEBUG
  char text[5];
  Serial.print("X : ");
  sprintf(text, "%04d", x);
  Serial.print(text);
  Serial.print(" Y : ");
  sprintf(text, "%04d", y);
  Serial.print(text);
  Serial.print(" mode : ");
  Serial.print(seq ? "SEQ" : "H");
  if (seq == true) {
    Serial.print(" X : ");
    Serial.print(SeqX);
    Serial.print(" Y : ");
    Serial.println(SeqY);
  } else {
    Serial.println(H);
  }
#endif
}