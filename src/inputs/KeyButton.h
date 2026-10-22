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
    bool reading = digitalRead(_pin) == LOW;
    uint32_t now = millis();

    if (reading != _lastReading)
    { // pin changed: restart the timer
      _lastReading = reading;
      _lastMs = now;
    }

    // Accept the new state once it has been stable for DEBOUNCE_MS
    if (now - _lastMs >= DEBOUNCE_MS && reading != _wasPressed)
    {
      _wasPressed = reading;
      if (_wasPressed && _cb)
        _cb(); // fire once per press, on the settled press
    }
  }

private:
  static constexpr uint32_t DEBOUNCE_MS = 20;
  uint8_t _pin;
  Callback _cb;
  bool _wasPressed = false;
  bool _lastReading = false;
  uint32_t _lastMs = 0;
};