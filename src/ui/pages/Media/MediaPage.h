
#ifndef DISPLAYUI_H
#define DISPLAYUI_H

#include <TFT_eSPI.h>
#include <cstdint>
#include "../IPage.h"

class MediaPage : public IPage
{
public:
    MediaPage(uint8_t id, TFT_eSPI &tft) : IPage(id, tft) {}
    void drawStatic() override { drawStaticLayout(); };
    void updateTrack(const char *title, const char *artist, const char *album);
    void updateProgress(int currentSec, int totalSec);
    void drawAlbumArt(int x, int y, int w, int h);

private:
    void drawStaticLayout();
    void clearMetadataArea();
};

#endif
