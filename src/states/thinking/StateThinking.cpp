#include "StateThinking.h"
#include <Arduino.h>
#include <math.h>
#include "../../JarvisConfig.h"
#include "../../LedRing.h"

namespace {
void renderHighlight(int center, int radius, float backgroundIntensity, float peakIntensity) {
  // Render one soft highlight centred on the supplied LED position.
  for (int offset = -radius; offset <= radius; offset++) {
    const int distance = abs(offset);
    float fade = 0.5f * (1.0f + cos(PI * distance / radius));
    fade = pow(fade, 1.2f);
    const float intensity = backgroundIntensity
      + fade * (peakIntensity - backgroundIntensity);
    const int pixel = (center + offset + LED_COUNT) % LED_COUNT;
    ledRing.setPixelColor(pixel, ledRing.scaledColor(baseR, baseG, baseB, intensity));
  }
}

void updateThinkingMode(bool pulsating, bool duo) {
  // THINKING rotates one highlight over a dim background. The pulsating and
  // duo variants reuse this renderer with different highlight parameters.
  const unsigned long frameTime = 20;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();
  const float backgroundIntensity = 0.08f;
  const float pulse = (sin(animationStep * 0.14f) + 1.0f) / 2.0f;
  // Give THINKING_PULSATING a much wider brightness range than regular THINKING.
  const float peakIntensity = pulsating ? 0.12f + pulse * 0.88f : 1.0f;
  // The duo variant is intentionally sparse: only its two highlights are lit.
  const float highlightBackground = duo ? 0.0f : backgroundIntensity;
  if (duo) ledRing.clear();
  else ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, backgroundIntensity));
  // The duo mode moves a little faster so the two opposite highlights feel
  // more dynamic without changing the regular Thinking modes.
  const float rotationSpeed = duo ? 1.25f : 1.0f;
  const int center = (int)(animationStep * rotationSpeed) % LED_COUNT;
  const int radius = duo ? 8 : 16;
  renderHighlight(center, radius, highlightBackground, peakIntensity);
  if (duo) {
    renderHighlight((center + LED_COUNT / 2) % LED_COUNT,
                    radius, highlightBackground, peakIntensity);
  }
  ledRing.show();
  animationStep++;
}
}

void updateThinking() { updateThinkingMode(false, false); }
void updateThinkingPulsating() { updateThinkingMode(true, false); }
void updateThinkingDuo() { updateThinkingMode(false, true); }

