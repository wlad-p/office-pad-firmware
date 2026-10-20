#include "ui/App.h"
#include "pages/PageID.h"

void App::init()
{
    // clock
    rtc_init();

    // UI
    tft_.begin();
    tft_.setRotation(1);
    tft_.fillScreen(TFT_BLACK);
};

void App::update(){
    if(current_) current_->update();
}

void App::sync(int16_t year,
               int8_t month,
               int8_t day,
               int8_t dotw,
               int8_t hour,
               int8_t min,
               int8_t sec)
{
    datetime_t dateTime = {
        .year = year,
        .month = month,
        .day = day,
        .dotw = dotw,
        .hour = hour,
        .min = min,
        .sec = sec};

    rtc_set_datetime(&dateTime);
}

datetime_t App::dateTime() const {
    datetime_t dt;
    rtc_get_datetime(&dt);
    return dt;
}

void App::switchTo(PageID id)
{
    current_ = pageForID(id);
    if (current_)
        current_->drawStatic();
}

IPage *App::pageForID(PageID id)
{
    switch (id)
    {
    case STARTUP_PAGE:
        return &startupPage_;
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

PageID App::currentPageID()
{
    return pageIdFromInt(current_->id());
}