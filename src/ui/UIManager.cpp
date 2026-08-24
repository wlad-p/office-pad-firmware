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
    // _tft.fillScreen(TFT_BLACK); // clear between pages
    _current = pageForID(id);
    if (_current)
        _current->drawStatic(_tft);
}

IPage *UIManager::pageForID(PageID id)
{
    switch (id)
    {
    case STARTUP_PAGE:
        return &_startupPage;
    case MEDIA_PAGE:
        return &_mediaPage;
    case SYSTEM_PAGE:
        return &_systemPage;
    case CLOCK_PAGE:
        return &_clockPage;
    case CALENDAR_PAGE:
        return &_calendarPage;
    default:
        return nullptr;
    }
}

PageID UIManager::currentPageID(){
    return pageIdFromInt(_current->id());
}