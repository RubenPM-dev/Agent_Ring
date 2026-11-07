#include "StateOff.h"
#include <Arduino.h>
#include "LedRing.h"

void updateOff() {
  static unsigned long lastOffUpdate = 0;
  if (millis() - lastOffUpdate < 500) return;
  lastOffUpdate = millis();
  ledRing.clear();
  ledRing.show();
}

