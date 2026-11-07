#include "LedRing.h"
#include "JarvisConfig.h"

LedRing ledRing;

LedRing::LedRing()
  : pixels(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800) {}

void LedRing::begin() {
  pixels.begin();
  pixels.setBrightness(255);
}

void LedRing::clear() { pixels.clear(); }
void LedRing::fill(uint32_t color) { pixels.fill(color); }
void LedRing::setPixelColor(int pixel, uint32_t color) { pixels.setPixelColor(pixel, color); }
void LedRing::show() { pixels.show(); }

uint32_t LedRing::scaledColor(int r, int g, int b, float effectIntensity) const {
  effectIntensity = constrain(effectIntensity, 0.0f, 1.0f);
  const float finalScale = effectIntensity * (userBrightness / 100.0f);
  const int finalR = constrain((int)(r * finalScale), 0, 255);
  const int finalG = constrain((int)(g * finalScale), 0, 255);
  const int finalB = constrain((int)(b * finalScale), 0, 255);
  return Adafruit_NeoPixel::Color(finalR, finalG, finalB);
}

