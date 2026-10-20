#include "network/NetworkInterface.h"
#include "Config.h"
#include "ui/App.h"
#include "ui/pages/PageID.h"
#include "NetworkCommands.h"
#include "ui/pages/Media/LayoutConfig.h"
#include "ui/pages/Calendar/CalendarEvent.h"

// ############### OUTGOING #####################

void NetworkInterface::sendJson(const JsonDocument &doc)
{
    String output;
    serializeJson(doc, output);
    Serial.print(output);
    Serial.print('\n');
}

void NetworkInterface::sendVolumeUp()
{
    JsonDocument doc;
    doc["btn"] = "volume_up";
    sendJson(doc);
};

void NetworkInterface::sendVolumeDown()
{
    JsonDocument doc;
    doc["btn"] = "volume_down";
    sendJson(doc);
};

void NetworkInterface::sendToggleMute()
{
    JsonDocument doc;
    doc["btn"] = "toggle_mute";
    sendJson(doc);
}

void NetworkInterface::sendPage()
{
    JsonDocument doc;
    doc["btn"] = "page";
    sendJson(doc);
};

void NetworkInterface::sendToggleMic()
{
    JsonDocument doc;
    doc["btn"] = "toggle_mic";
    sendJson(doc);
}

void NetworkInterface::sendScreenshot()
{
    JsonDocument doc;
    doc["btn"] = "screenshot";
    sendJson(doc);
}

void NetworkInterface::sendLock()
{
    JsonDocument doc;
    doc["btn"] = "lock";
    sendJson(doc);
}

void NetworkInterface::sendF1()
{
    JsonDocument doc;
    doc["btn"] = "f1";
    sendJson(doc);
}

void NetworkInterface::sendF2()
{
    JsonDocument doc;
    doc["btn"] = "f2";
    sendJson(doc);
}

void NetworkInterface::sendF3()
{
    JsonDocument doc;
    doc["btn"] = "f3";
    sendJson(doc);
}

void NetworkInterface::sendF4()
{
    JsonDocument doc;
    doc["btn"] = "f4";
    sendJson(doc);
}

void NetworkInterface::sendPrev()
{
    JsonDocument doc;
    doc["btn"] = "prev";
    sendJson(doc);
}

void NetworkInterface::sendTogglePause()
{
    JsonDocument doc;
    doc["btn"] = "toggle_pause";
    sendJson(doc);
}

void NetworkInterface::sendNext()
{
    JsonDocument doc;
    doc["btn"] = "next";
    sendJson(doc);
}

// ############### INCOMING #####################

void NetworkInterface::handleIncomingData(String &jsonPayload)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error)
    {
        return;
    }

    // Set page
    const char *pageString = doc["page"];
    PageID page = pageIdFromString(pageString);
    PageID currentPageID = App::instance().currentPageID();
    if (currentPageID != page)
    {
        App::instance().switchTo(page);
        return;
    }

    // Run command
    const char *cmdRaw = doc["cmd"];
    if (!cmdRaw)
        return;
    const auto cmd = NetworkCommandFromString(cmdRaw);

    switch (cmd)
    {
    case TRACK:
        handleTrack(doc);
        break;
    case CLOCK:
        handleClock(doc);
        // break;
    case PROGRESS:
        handleProgress(doc);
        break;
    case MEDIA_ART:
        handleMediaArt(doc);
        break;
    case CALENDAR:
        handleCalendar(doc);
        break;
    case SYSTEM:
        handleSystem(doc);
        break;
    case UNKNOWN:
        break;
    default:
        break;
    };
}

void NetworkInterface::handleCalendar(const JsonDocument &doc)
{
    const size_t MAX_EVENTS = 3;

    JsonArrayConst eventList = doc["events"].as<JsonArrayConst>();
    CalendarEvent calendarEvents[MAX_EVENTS];

    size_t NUM_EVENTS = eventList.size();
    for (int i=0; i < NUM_EVENTS; i++)
    {
        const char *title = eventList[i]["title"];
        const char *location = eventList[i]["location"];
        uint16_t year = eventList[i]["year"].as<uint16_t>();
        uint8_t month = eventList[i]["month"].as<uint8_t>();
        uint8_t day = eventList[i]["day"].as<uint8_t>();
        uint8_t hour = eventList[i]["hour"].as<uint8_t>();
        uint8_t min = eventList[i]["min"].as<uint8_t>();
        bool allDay = eventList[i]["all_day"].as<bool>();

        CalendarEvent event{.title = title, .location = location, .year = year, .month = month, .day = day, .hour = hour, .min = min, .allDay = allDay};
        calendarEvents[i] = event;
    }

    App::instance().calendarPage().updateEvents(calendarEvents, NUM_EVENTS);
}

void NetworkInterface::handleClock(const JsonDocument &doc)
{

    uint16_t year = doc["year"].as<uint16_t>();
    uint8_t month = doc["month"].as<uint8_t>();
    uint8_t day = doc["day"].as<uint8_t>();
    uint8_t dotw = doc["dotw"].as<uint8_t>();
    uint8_t hour = doc["hour"].as<uint8_t>();
    uint8_t min = doc["min"].as<uint8_t>();
    uint8_t sec = doc["sec"].as<uint8_t>();

    App::instance().sync(year, month, day, dotw, hour, min, sec);
}

void NetworkInterface::handleTrack(const JsonDocument &doc)
{

    const char *title = doc["title"] | "Unknown Title";
    const char *artist = doc["artist"] | "Unknown Artist";
    const char *album = doc["album"] | "";
    float pos = doc["pos"].as<float>();
    float duration = doc["dur"].as<float>();

    int currentDurationSec = (int)duration;

    App::instance().mediaPage().updateTrack(title, artist, album);
    App::instance().mediaPage().updateProgress(pos, currentDurationSec);
}

void NetworkInterface::handleProgress(const JsonDocument &doc)
{
    float pos = doc["pos"].as<float>();
    float duration = doc["dur"].as<float>();
    App::instance().mediaPage().updateProgress((int)pos, (int)duration);
}

void NetworkInterface::handleMediaArt(const JsonDocument &doc)
{
    Serial.println("handle media art");

    long expectedSize = doc["size"] | 0;
    if (expectedSize == 39200)
    {
        receiveAndDrawArt();
    }
}
void NetworkInterface::handleSystem(const JsonDocument &doc)
{
    SystemStats _stats;
    _stats.cpuPercent = doc["cpu_percent"].as<float>();
    _stats.ramTotalGB = doc["ram_total_gb"].as<float>();
    _stats.ramUsedGB = doc["ram_used_gb"].as<float>();
    _stats.swapTotalGB = doc["swap_total_gb"].as<float>();
    _stats.swapUsedGB = doc["swap_used_gb"].as<float>();
    _stats.diskTotalGB = doc["disk_total_gb"].as<float>();
    _stats.diskUsedGB = doc["disk_used_gb"].as<float>();
    _stats.valid = true;

    App::instance().systemPage().updateStats(_stats);
}

void NetworkInterface::receiveAndDrawArt()
{

    Serial.println("receive and draw art");
    const int x = UI::artX;
    const int y = UI::artY;
    const int w = UI::artSize;
    const int h = UI::artSize;
    const long totalBytes = (long)w * h * 2;

    // Draw placeholder / background while we receive
    // App::instance().mediaPage().drawAlbumArt(x, y, w, h);

    // --- Create sprite ---
    TFT_eSprite sprite(&App::instance().tft()); // App::instance().tft() must return TFT_eSPI&
    sprite.setColorDepth(16);

    if (!sprite.createSprite(w, h))
    {
        // Fallback: not enough RAM, draw pixel by pixel
        // long bytesRead = 0;
        // App::instance()nt8_t buf[2];
        // App::instance().mediaPage().setAddrWindow(x, y, x + w - 1, y + h - 1);
        // while (bytesRead < totalBytes) {
        //     if (Serial.available() >= 2) {
        //         Serial.readBytes(buf, 2);
        //         App::instance()nt16_t color = (buf[0] << 8) | buf[1];
        //         App::instance().mediaPage().pushColor(color);
        //         bytesRead += 2;
        //     }
        // }
        return;
    }

    // --- Receive directly into sprite's raw buffer ---
    uint16_t *spriteBuffer = (uint16_t *)sprite.getPointer();
    long pixelsReceived = 0;
    const long totalPixels = (long)w * h;
    uint8_t buf[2];

    while (pixelsReceived < totalPixels)
    {
        if (Serial.available() >= 2)
        {
            Serial.readBytes(buf, 2);
            spriteBuffer[pixelsReceived++] = (buf[0] << 8) | buf[1];
        }
    }

    // --- Push entire frame to display in one shot ---
    sprite.pushSprite(x, y);
    sprite.deleteSprite();
}