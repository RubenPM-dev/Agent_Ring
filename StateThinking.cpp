#include "StateThinking.h"
#include <Arduino.h>
#include <math.h>
#include "JarvisConfig.h"
#include "LedRing.h"

void updateThinking() {
  const unsigned long frameTime = 20;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();
  const float backgroundIntensity = 0.08f;
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, backgroundIntensity));
  const int center = animationStep % LED_COUNT;
  const int radius = 16;
  for (int offset = -radius; offset <= radius; offset++) {
    const int distance = abs(offset);
    float fade = 0.5f * (1.0f + cos(PI * distance / radius));
    fade = pow(fade, 1.2f);
    const float intensity = backgroundIntensity + fade * (1.0f - backgroundIntensity);
    const int pixel = (center + offset + LED_COUNT) % LED_COUNT;
    ledRing.setPixelColor(pixel, ledRing.scaledColor(baseR, baseG, baseB, intensity));
  }
  ledRing.show();
  animationStep++;
}

