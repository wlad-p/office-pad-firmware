#pragma once
#include <Arduino.h>

class RotaryEncoder {
public:
  using Callback = void(*)();

  RotaryEncoder(uint8_t pinA, uint8_t pinB,
                Callback onCW, Callback onCCW)
    : _pinA(pinA), _pinB(pinB),
      _onCW(onCW), _onCCW(onCCW) {}

  void begin() {
    pinMode(_pinA, INPUT_PULLUP);
    pinMode(_pinB, INPUT_PULLUP);
    // pinMode(_pinC, INPUT_PULLUP);  // C is the common ground, usually just GND
    _lastA = digitalRead(_pinA);
  }

  void poll() {
    bool A = digitalRead(_pinA);
    if (A != _lastA) {                  // A changed — a detent is happening
      bool B = digitalRead(_pinB);
      if (A == LOW) {                   // falling edge of A
        if (B == HIGH) _onCW();         // B hasn't changed yet → clockwise
        else           _onCCW();        // B already changed → counter-clockwise
      }
      _lastA = A;
    }
  }

private:
  uint8_t  _pinA, _pinB, _pinC;
  Callback _onCW, _onCCW;
  bool     _lastA = HIGH;
};