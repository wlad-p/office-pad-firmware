#pragma once
#include "ui/UIManager.h"
#include "NetworkCommands.h"
#include <ArduinoJson.h>

class NetworkInterface
{

public:
    static void handleIncomingData(UIManager &ui, String &jsonPayload);
    static void sendVolumeUp();
    static void sendVolumeDown();
    static void sendToggleMute();
    static void receiveAndDrawArt(UIManager &ui);

    static void sendPage();
    static void sendToggleMic();
    static void sendScreenshot();
    static void sendLock();

    static void sendF1();
    static void sendF2();
    static void sendF3();
    static void sendF4();

    static void sendPrev();
    static void sendTogglePause();
    static void sendNext();
private:
    static int _currentDurationSec;

    static void sendJson(const StaticJsonDocument<128> &doc);

    static void handleTrack(const StaticJsonDocument<512> &doc,  UIManager &ui);
    static void handleProgress(const StaticJsonDocument<512> &doc,  UIManager &ui);
    static void handleMediaArt(const StaticJsonDocument<512> &doc,  UIManager &ui);
    static void handleSystem(const StaticJsonDocument<512> &doc, UIManager& ui);
    static void handleClock(const StaticJsonDocument<512> &doc, UIManager& ui);
};