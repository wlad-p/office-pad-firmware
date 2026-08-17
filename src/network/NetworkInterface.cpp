#include "network/NetworkInterface.h"
#include "Config.h"
#include "ui/UIManager.h"
#include "pages/PageID.h"
#include "NetworkCommands.h"

// ############### OUTGOING #####################

void NetworkInterface::sendJson(const StaticJsonDocument<128> &doc)
{
    String output;
    serializeJson(doc, output);
    Serial.print(output);
    Serial.print('\n');
}

void NetworkInterface::sendVolumeUp()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "volume_up";
    sendJson(doc);
};

void NetworkInterface::sendVolumeDown()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "volume_down";
    sendJson(doc);
};

void NetworkInterface::sendToggleMute()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "toggle_mute";
    sendJson(doc);
}

void NetworkInterface::sendPage()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "page";
    sendJson(doc);
};

void NetworkInterface::sendToggleMic()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "toggle_mic";
    sendJson(doc);
}

void NetworkInterface::sendScreenshot()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "screenshot";
    sendJson(doc);
}

void NetworkInterface::sendLock()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "lock";
    sendJson(doc);
}

void NetworkInterface::sendF1()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "f1";
    sendJson(doc);
}

void NetworkInterface::sendF2()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "f2";
    sendJson(doc);
}

void NetworkInterface::sendF3()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "f3";
    sendJson(doc);
}

void NetworkInterface::sendF4()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "f4";
    sendJson(doc);
}

void NetworkInterface::sendPrev()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "prev";
    sendJson(doc);
}

void NetworkInterface::sendTogglePause()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "toggle_pause";
    sendJson(doc);
}

void NetworkInterface::sendNext()
{
    StaticJsonDocument<128> doc;
    doc["btn"] = "next";
    sendJson(doc);
}

// ############### INCOMING #####################

void NetworkInterface::handleIncomingData(UIManager &ui, String &jsonPayload)
{
    StaticJsonDocument<512> doc;
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
    case PROGRESS:
        handleProgress(doc, ui);
        break;
    case ART_SIZE:
        handleArtSize(doc, ui);
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


void NetworkInterface::handleTrack(const StaticJsonDocument<512> &doc, UIManager &ui)
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

void NetworkInterface::handleProgress(const StaticJsonDocument<512> &doc, UIManager &ui)
{
    float pos = doc["pos"].as<float>();

    if (doc.containsKey("dur"))
    {
        float duration = doc["dur"].as<float>();
        int currentDurationSec = (int)duration;
    }

    // TODO
    ui.mediaPage().updateProgress((int)pos, 0);
}

void NetworkInterface::handleArtSize(const StaticJsonDocument<512> &doc, UIManager &ui)
{
    long expectedSize = doc["size"] | 0;
    if (expectedSize == 39200)
    {
        // receiveAndDrawArt(ui);
    }
}
void NetworkInterface::handleSystem(const StaticJsonDocument<512> &doc, UIManager &ui)
{
    SystemStats _stats;
    _stats.cpuPercent = doc["cpu_percent"] | 0;
    _stats.ramTotalBytes = atoll(doc["ram_total_bytes"] | "0");
    _stats.ramUsedBytes = atoll(doc["ram_used_bytes"] | "0");
    _stats.swapTotalBytes = atoll(doc["swap_total_bytes"] | "0");
    _stats.swapUsedBytes = atoll(doc["swap_used_bytes"] | "0");
    _stats.diskTotalBytes = atoll(doc["disk_total_bytes"] | "0");
    _stats.diskUsedBytes = atoll(doc["disk_used_bytes"] | "0");
    _stats.valid = true;

    ui.systemPage().updateStats(_stats);
}

//
// void receiveAndDrawArt(UIManager& ui) {
//  int x = Layout::artX;
//  int y = Layout::M_artY;
//  int w = Layout::M_artSize;
//  int h = Layout::M_artSize;
//
//  ui.mediaPlayer().drawAlbumArt(x, y, w, h);
//
//  long bytesRead = 0;
//  uint8_t buffer[2]; // Temp buffer to hold 1 pixel (2 bytes)
//
//  while (bytesRead < 39200) {
//    if (Serial.available() >= 2) {
//      Serial.readBytes(buffer, 2);
//
//      // Reconstruct pixel color
//      uint16_t color = (buffer[0] << 8) | buffer[1];
//
//      // Write pixel immediately to the display controller's RAM
//      ui.mediaPlayer().pushColor(color);
//      bytesRead += 2;
//    }
//  }
//}