#include "ui/UIManager.h"
#include "pages/PageID.h"


void UIManager::init()
{
    _tft.begin();
    _tft.setRotation(1);
    _tft.fillScreen(TFT_BLACK);
};

void UIManager::resetDisplay()
{
    _tft.setAddrWindow(0, 0, Layout::screenWidth, Layout::screenHeight);
};

void UIManager::switchTo(PageID id)
{
    _tft.fillScreen(TFT_BLACK); // clear between pages
    _current = pageForID(id);
    if (_current)
        _current->drawStatic(_tft);
}

IPage *UIManager::pageForID(PageID id)
{
    switch (id)
    {
    case MEDIA_PAGE:
        return &mediaPage_;
    case SYSTEM_PAGE:
        return &systemPage_;
    case CLOCK_PAGE:
        return &clockPage_;
    case CALENDAR_PAGE:
        return &calendarPage_;
    default:
        return nullptr;
    }
}

PageID UIManager::currentPageID(){
    return pageIdFromInt(_current->id());
}