#include "network/NetworkInterface.h"
#include "Config.h"
#include "inputs/inputs.h"
#include "ui/UIManager.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();
UIManager ui(tft);
String serialBuffer = "";


void setup() {

  Serial.begin(115200);
  inputsBegin();

  pinMode(TFT_BL, OUTPUT);

  ui.init();
  ui.switchTo(SYSTEM_PAGE);

  digitalWrite(TFT_BL, HIGH);

}

void loop() {
  
  inputsPoll();

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {// End of packet reached, parse it
      NetworkInterface::handleIncomingData(ui, serialBuffer);
      serialBuffer = "";
    } else {
      serialBuffer += c; // Append to packet
    }
  }
}