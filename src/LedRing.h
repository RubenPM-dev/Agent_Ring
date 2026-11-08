#pragma once

#include <Adafruit_NeoPixel.h>
#include "JarvisConfig.h"

class LedRing {
public:
  LedRing();
  void begin();
  void clear();
  void fill(uint32_t color);
  void setPixelColor(int pixel, uint32_t color);
  void show();
  uint32_t scaledColor(int r, int g, int b, float effectIntensity) const;
  void startTransition(unsigned long durationMs);
private:
  Adafruit_NeoPixel pixels;
  uint32_t transitionColors[LED_COUNT];
  unsigned long transitionStarted = 0;
  unsigned long transitionDuration = 0;
};

extern LedRing ledRing;

