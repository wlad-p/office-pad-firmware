#ifndef IPAGE_H
#define IPAGE_H

#include <TFT_eSPI.h>
#include <cstdint>

class IPage
{
public:
    uint8_t id() const { return id_; };
    virtual void drawStatic(TFT_eSPI &tft) = 0; // chrome, called once on switch
    explicit IPage(uint8_t id, TFT_eSPI &tft) : id_(id), _tft(tft) {};
    virtual ~IPage() = default;

private:
    uint8_t id_;

protected:
    TFT_eSPI &_tft;
};

#endif
