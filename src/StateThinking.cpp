#include "StateThinking.h"
#include <Arduino.h>
#include <math.h>
#include "JarvisConfig.h"
#include "LedRing.h"

namespace {
void updateThinkingMode(bool pulsating) {
  const unsigned long frameTime = 20;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();
  const float backgroundIntensity = 0.08f;
  const float pulse = (sin(animationStep * 0.10f) + 1.0f) / 2.0f;
  const float peakIntensity = pulsating ? 0.35f + pulse * 0.65f : 1.0f;
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, backgroundIntensity));
  const int center = animationStep % LED_COUNT;
  const int radius = 16;
  for (int offset = -radius; offset <= radius; offset++) {
    const int distance = abs(offset);
    float fade = 0.5f * (1.0f + cos(PI * distance / radius));
    fade = pow(fade, 1.2f);
    const float intensity = backgroundIntensity
      + fade * (peakIntensity - backgroundIntensity);
    const int pixel = (center + offset + LED_COUNT) % LED_COUNT;
    ledRing.setPixelColor(pixel, ledRing.scaledColor(baseR, baseG, baseB, intensity));
  }
  ledRing.show();
  animationStep++;
}
}

void updateThinking() { updateThinkingMode(false); }
void updateThinkingPulsating() { updateThinkingMode(true); }

