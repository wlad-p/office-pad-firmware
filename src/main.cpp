#include "network/NetworkInterface.h"
#include "Config.h"
#include "inputs/inputs.h"
#include "ui/App.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <TFT_eSPI.h>

String serialBuffer = "";


void setup() {

  Serial.begin(115200);
  inputsBegin();

App::instance().init();
  pinMode(TFT_BL, OUTPUT);

  App::instance().switchTo(STARTUP_PAGE);

  digitalWrite(TFT_BL, HIGH);

}

void loop() {
  
  App::instance().update();
  inputsPoll();

  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {// End of packet reached, parse it
      NetworkInterface::handleIncomingData(serialBuffer);
      serialBuffer = "";
    } else {
      serialBuffer += c; // Append to packet
    }
  }
}