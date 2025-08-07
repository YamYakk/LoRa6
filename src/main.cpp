#include <Arduino.h>
#include <RadioLib.h>
#include "config_node5.h"


static uint16_t uplinkCounter = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  // radio init (uses default VSPI)
  int16_t st = radio.begin(915000000);
  debug(st != RADIOLIB_ERR_NONE, F("Init radio failed"), st, true);

  // OTAA join
  st = node.beginOTAA(joinEUI, devEUI, nwkKey, appKey);
  debug(st != RADIOLIB_ERR_NONE, F("Init node failed"), st, true);
  st = node.activateOTAA();
  debug(st != RADIOLIB_LORAWAN_NEW_SESSION, F("Join failed"), st, true);

  Serial.println(F("Ready!"));
}

void loop() {
  // 1) read hall sensor value
  int16_t hallValue = hallRead();
  int16_t t100 = hallValue; // or scale as needed

  // 2) grab counter
  uint16_t cnt = uplinkCounter++;

  Serial.print(F("Cnt=")); Serial.print(cnt);
  Serial.print(F("  Hall=")); Serial.print(hallValue); Serial.println(F(" units"));

  // 3) pack into 4 bytes: [cnt_H, cnt_L, t100_H, t100_L]
  uint8_t payload[4] = {
    highByte(cnt), lowByte(cnt),
    highByte(t100), lowByte(t100)
  };

  // 4) send uplink on port 1
  int16_t rc = node.sendUplink(payload, sizeof(payload), 1);
  debug(rc != RADIOLIB_ERR_NONE, F("Uplink failed"), rc, false);

  Serial.print(F("Next in "));
  Serial.print(uplinkIntervalSeconds);
  Serial.println(F(" s\n"));

  delay(uplinkIntervalSeconds * 1000UL);
}
