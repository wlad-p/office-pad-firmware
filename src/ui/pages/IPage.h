#ifndef IPAGE_H
#define IPAGE_H

#include <TFT_eSPI.h>
#include <cstdint>

class IPage
{
public:
    uint8_t id() const { return id_; };
    virtual void drawStatic() = 0; // chrome, called once on switch
    virtual void update(){};
    explicit IPage(uint8_t id, TFT_eSPI &tft) : id_(id), tft_(tft) {};
    virtual ~IPage() = default;

private:
    uint8_t id_;

protected:
    TFT_eSPI &tft_;
};

#endif
