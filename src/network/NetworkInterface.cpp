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
        // return;
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
        break;
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

static void copyText(char *dst, size_t size, const char *src)
{
    strncpy(dst, src ? src : "", size - 1);
    dst[size - 1] = '\0';
}

static void readEvent(JsonObjectConst e, CalendarEvent &out)
{
    copyText(out.title, sizeof(out.title), e["title"] | "");
    copyText(out.location, sizeof(out.location), e["location"] | "");
    out.startYear = e["start_year"] | 0;
    out.startMonth = e["start_month"] | 0;
    out.startDay = e["start_day"] | 0;
    out.startHour = e["start_hour"] | 0;
    out.startMin = e["start_min"] | 0;
    out.endYear = e["end_year"] | 0;
    out.endMonth = e["end_month"] | 0;
    out.endDay = e["end_day"] | 0;
    out.endHour = e["end_hour"] | 0;
    out.endMin = e["end_min"] | 0;
    out.allDay = e["all_day"] | false;
}

void NetworkInterface::handleCalendar(const JsonDocument &doc)
{
    static CalendarEvent events[CalendarPage::MAX_EVENTS];   // static: ~1 KB, keep it off the stack
    size_t count = 0;

    JsonVariantConst list = doc["events"];
    if (list.is<JsonObjectConst>())
    {
        for (JsonPairConst kv : list.as<JsonObjectConst>())
        {
            if (count >= CalendarPage::MAX_EVENTS) break;
            events[count] = CalendarEvent{};
            readEvent(kv.value().as<JsonObjectConst>(), events[count++]);
        }
    }
    else if (list.is<JsonArrayConst>())
    {
        for (JsonObjectConst e : list.as<JsonArrayConst>())
        {
            if (count >= CalendarPage::MAX_EVENTS) break;
            events[count] = CalendarEvent{};
            readEvent(e, events[count++]);
        }
    }

    App::instance().calendarPage().updateEvents(events, count);
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