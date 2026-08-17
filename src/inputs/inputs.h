#pragma once
#include "KeyButton.h"
#include "Config.h"
#include "RotaryEncoder.h"
#include "network/NetworkInterface.h"


KeyButton btnMic(BTN_PIN_MIC, NetworkInterface::sendToggleMic);
KeyButton btnMode(BTN_PIN_MODE, NetworkInterface::sendPage);
KeyButton btnScreenshot(BTN_PIN_SCREENSHOT, NetworkInterface::sendScreenshot);
KeyButton btnLock(BTN_PIN_LOCK, NetworkInterface::sendLock);

KeyButton btnF1(BTN_PIN_F1, NetworkInterface::sendF1);
KeyButton btnF2(BTN_PIN_F2, NetworkInterface::sendF2);
KeyButton btnF3(BTN_PIN_F3, NetworkInterface::sendF3);
KeyButton btnF4(BTN_PIN_F4, NetworkInterface::sendF4);

KeyButton btnPrev(BTN_PIN_PREV, NetworkInterface::sendPrev);
KeyButton btnPause(BTN_PIN_PAUSE, NetworkInterface::sendTogglePause);
KeyButton btnNext(BTN_PIN_NEXT, NetworkInterface::sendNext);

inline RotaryEncoder encoder(PIN_ENC_A, PIN_ENC_B, NetworkInterface::sendVolumeUp, NetworkInterface::sendVolumeDown);

inline KeyButton allButtons[] = {
    btnMic, btnMode, btnScreenshot, btnLock, btnF1, btnF2, btnF3, btnF4, btnPrev, btnPause, btnNext};

inline constexpr uint8_t BUTTON_COUNT = sizeof(allButtons) / sizeof(allButtons[0]);

inline void inputsBegin()
{
    for (auto &b : allButtons)
        b.begin();
    encoder.begin();
}
inline void inputsPoll()
{
    for (auto &b : allButtons)
        b.poll();
    encoder.poll();
}