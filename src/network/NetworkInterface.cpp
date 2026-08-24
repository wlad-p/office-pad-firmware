#include "network/NetworkInterface.h"
#include "Config.h"
#include "ui/UIManager.h"
#include "ui/pages/PageID.h"
#include "NetworkCommands.h"
#include "ui/pages/Media/LayoutConfig.h"

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

void NetworkInterface::handleIncomingData(UIManager &ui, String &jsonPayload)
{
    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, jsonPayload);

    if (error)
    {
        return;
    }

    // Set page
    const char* pageString = doc["page"];
    PageID page = pageIdFromString(pageString);
    PageID currentPageID = ui.currentPageID();
    if(currentPageID != page){
        ui.switchTo(page);
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
        handleTrack(doc, ui);
        break;
    case CLOCK:
        handleClock(doc, ui);
    case PROGRESS:
        handleProgress(doc, ui);
        break;
    case MEDIA_ART:
        handleMediaArt(doc, ui);
        break;
    case SYSTEM:
        handleSystem(doc, ui);
        break;
    case UNKNOWN:
        break;
    default:
        break;
    };
}

void NetworkInterface::handleClock(const JsonDocument &doc, UIManager &ui){

}


void NetworkInterface::handleTrack(const JsonDocument &doc, UIManager &ui)
{

    ui.switchTo(MEDIA_PAGE);
    const char *title = doc["title"] | "Unknown Title";
    const char *artist = doc["artist"] | "Unknown Artist";
    const char *album = doc["album"] | "";
    float pos = doc["pos"].as<float>();
    float duration = doc["dur"].as<float>();

    int currentDurationSec = (int)duration;

    ui.mediaPage().updateTrack(title, artist, album);
    ui.mediaPage().updateProgress(pos, currentDurationSec);
}

void NetworkInterface::handleProgress(const JsonDocument &doc, UIManager &ui)
{
    float pos = doc["pos"].as<float>();
    float duration = doc["dur"].as<float>();
    ui.mediaPage().updateProgress((int)pos, (int)duration);
}

void NetworkInterface::handleMediaArt(const JsonDocument &doc, UIManager &ui)
{
    Serial.println("handle media art");

    long expectedSize = doc["size"] | 0;
    if (expectedSize == 39200)
    {
        receiveAndDrawArt(ui);
    }
}
void NetworkInterface::handleSystem(const JsonDocument &doc, UIManager &ui)
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

    ui.systemPage().updateStats(_stats);
}


void NetworkInterface::receiveAndDrawArt(UIManager& ui) {

    Serial.println("receive and draw art");
    const int x = UI::artX;
    const int y = UI::artY;
    const int w = UI::artSize;
    const int h = UI::artSize;
    const long totalBytes = (long)w * h * 2;

    // Draw placeholder / background while we receive
    // ui.mediaPage().drawAlbumArt(x, y, w, h);

    // --- Create sprite ---
    TFT_eSprite sprite(&ui.tft()); // ui.tft() must return TFT_eSPI&
    sprite.setColorDepth(16);

    if (!sprite.createSprite(w, h)) {
        // Fallback: not enough RAM, draw pixel by pixel
        // long bytesRead = 0;
        // uint8_t buf[2];
        // ui.mediaPage().setAddrWindow(x, y, x + w - 1, y + h - 1);
        // while (bytesRead < totalBytes) {
        //     if (Serial.available() >= 2) {
        //         Serial.readBytes(buf, 2);
        //         uint16_t color = (buf[0] << 8) | buf[1];
        //         ui.mediaPage().pushColor(color);
        //         bytesRead += 2;
        //     }
        // }
        return;
    }

    // --- Receive directly into sprite's raw buffer ---
    uint16_t* spriteBuffer = (uint16_t*)sprite.getPointer();
    long pixelsReceived = 0;
    const long totalPixels = (long)w * h;
    uint8_t buf[2];

    while (pixelsReceived < totalPixels) {
        if (Serial.available() >= 2) {
            Serial.readBytes(buf, 2);
            spriteBuffer[pixelsReceived++] = (buf[0] << 8) | buf[1];
        }
    }

    // --- Push entire frame to display in one shot ---
    sprite.pushSprite(x, y);
    sprite.deleteSprite();
}