#pragma once
#include "ui/App.h"
#include "NetworkCommands.h"
#include <ArduinoJson.h>

class NetworkInterface
{

public:
    static void handleIncomingData(String &jsonPayload);
    static void sendVolumeUp();
    static void sendVolumeDown();
    static void sendToggleMute();
    static void receiveAndDrawArt();

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

    static void sendJson(const JsonDocument &doc);

    static void handleTrack(const JsonDocument &doc);
    static void handleProgress(const JsonDocument &doc);
    static void handleMediaArt(const JsonDocument &doc);
    static void handleSystem(const JsonDocument &doc);
    static void handleClock(const JsonDocument &doc);
    static void handleCalendar(const JsonDocument &doc);

};