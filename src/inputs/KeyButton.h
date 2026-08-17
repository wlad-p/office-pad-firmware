#pragma once
#include <Arduino.h>

class KeyButton
{
public:
  using Callback = void (*)();

  KeyButton(uint8_t pin, Callback cb)
      : _pin(pin), _cb(cb) {};

  void begin()
  {
    pinMode(_pin, INPUT_PULLUP);
  }

  void poll()
  {
    bool pressed = digitalRead(_pin) == LOW;
    if (pressed && !_wasPressed)
    {
      if (_cb)
        _cb();
      _wasPressed = false;
    }
    _wasPressed = pressed;
  }

private:
  uint8_t _pin;
  Callback _cb;
  bool _wasPressed = false;
  uint32_t _lastMs = 0;
};