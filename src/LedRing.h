#pragma once

#include <Adafruit_NeoPixel.h>

class LedRing {
public:
  LedRing();
  void begin();
  void clear();
  void fill(uint32_t color);
  void setPixelColor(int pixel, uint32_t color);
  void show();
  uint32_t scaledColor(int r, int g, int b, float effectIntensity) const;
private:
  Adafruit_NeoPixel pixels;
};

extern LedRing ledRing;

