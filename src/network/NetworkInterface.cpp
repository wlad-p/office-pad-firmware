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
    if (pageString)
    {
        PageID page = pageIdFromString(pageString);
        if (App::instance().currentPageID() != page)
            App::instance().switchTo(page);
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
    static CalendarEvent events[CalendarPage::MAX_EVENTS]; // static: ~1 KB, keep it off the stack
    size_t count = 0;

    JsonVariantConst list = doc["events"];
    if (list.is<JsonObjectConst>())
    {
        for (JsonPairConst kv : list.as<JsonObjectConst>())
        {
            if (count >= CalendarPage::MAX_EVENTS)
                break;
            events[count] = CalendarEvent{};
            readEvent(kv.value().as<JsonObjectConst>(), events[count++]);
        }
    }
    else if (list.is<JsonArrayConst>())
    {
        for (JsonObjectConst e : list.as<JsonArrayConst>())
        {
            if (count >= CalendarPage::MAX_EVENTS)
                break;
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

    Serial.println("{\"dbg\":\"track\"}");

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
    const int x = UI::artX;
    const int y = UI::artY;
    const int w = UI::artSize;
    const int h = UI::artSize;
    const size_t totalBytes = (size_t)w * h * 2;   // 39200

    TFT_eSprite sprite(&App::instance().tft());
    sprite.setColorDepth(16);
    uint8_t *dst = (uint8_t *)sprite.createSprite(w, h);

    // Python sends RGB565 high byte first, which is the byte order the sprite
    // stores internally, so the bytes are copied in unchanged.
    // Without a sprite (out of RAM) the bytes are still read and dropped,
    // so they don't end up in the JSON buffer.
    uint8_t scratch[64];
    size_t received = 0;
    uint32_t lastData = millis();

    while (received < totalBytes)
    {
        int n = Serial.available();
        if (n > 0)
        {
            size_t want = totalBytes - received;
            if ((size_t)n < want) want = n;
            if (!dst && want > sizeof(scratch)) want = sizeof(scratch);

            received += Serial.readBytes(dst ? dst + received : scratch, want);
            lastData = millis();
        }
        else if (millis() - lastData > 1000)
        {
            break;   // transfer stalled: give up instead of hanging
        }
    }

    if (dst)
    {
        if (received == totalBytes)
            sprite.pushSprite(x, y);
        sprite.deleteSprite();
    }
}