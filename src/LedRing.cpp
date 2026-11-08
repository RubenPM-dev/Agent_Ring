#include "LedRing.h"
#include <Arduino.h>
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

void LedRing::startTransition(unsigned long durationMs) {
  for (int i = 0; i < LED_COUNT; i++) transitionColors[i] = pixels.getPixelColor(i);
  transitionStarted = millis();
  transitionDuration = durationMs;
}

void LedRing::show() {
  if (transitionDuration > 0) {
    const unsigned long elapsed = millis() - transitionStarted;
    const float progress = elapsed >= transitionDuration
      ? 1.0f
      : (float)elapsed / (float)transitionDuration;

    for (int i = 0; i < LED_COUNT; i++) {
      const uint32_t oldColor = transitionColors[i];
      const uint32_t newColor = pixels.getPixelColor(i);
      const uint8_t red = (uint8_t)(((oldColor >> 16) & 0xFF) * (1.0f - progress)
        + ((newColor >> 16) & 0xFF) * progress);
      const uint8_t green = (uint8_t)(((oldColor >> 8) & 0xFF) * (1.0f - progress)
        + ((newColor >> 8) & 0xFF) * progress);
      const uint8_t blue = (uint8_t)((oldColor & 0xFF) * (1.0f - progress)
        + (newColor & 0xFF) * progress);
      pixels.setPixelColor(i, Adafruit_NeoPixel::Color(red, green, blue));
    }

    if (progress >= 1.0f) transitionDuration = 0;
  }
  pixels.show();
}

uint32_t LedRing::scaledColor(int r, int g, int b, float effectIntensity) const {
  effectIntensity = constrain(effectIntensity, 0.0f, 1.0f);
  const float finalScale = effectIntensity * (userBrightness / 100.0f);
  const int finalR = constrain((int)(r * finalScale), 0, 255);
  const int finalG = constrain((int)(g * finalScale), 0, 255);
  const int finalB = constrain((int)(b * finalScale), 0, 255);
  return Adafruit_NeoPixel::Color(finalR, finalG, finalB);
}

