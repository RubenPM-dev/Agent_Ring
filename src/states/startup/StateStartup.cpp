#include "StateStartup.h"
#include <Arduino.h>
#include <math.h>
#include "../../JarvisConfig.h"
#include "../JarvisState.h"
#include "../../LedRing.h"

namespace {
// Set true to test the full fill animation. Keep false for blink-only testing.
constexpr bool STARTUP_FILL_ENABLED = true;
bool fillComplete = false;
unsigned long blinkStarted = 0;
}

// Reset the startup phases so the animation can run again from the beginning.
void resetStartup() {
  fillComplete = !STARTUP_FILL_ENABLED;
  blinkStarted = millis();
}

void updateStartup() {
  const unsigned long frameTime = 25;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();

  // Phase 1: optionally add one LED per frame until the ring is full.
  if (STARTUP_FILL_ENABLED && !fillComplete) {
    ledRing.clear();
    const int pixelsToLight = min((int)animationStep + 1, (int)LED_COUNT);
    for (int i = 0; i < pixelsToLight; i++) {
      ledRing.setPixelColor(i, ledRing.scaledColor(baseR, baseG, baseB, 1.0f));
    }
    ledRing.show();
    animationStep++;

    if (pixelsToLight >= LED_COUNT) {
      fillComplete = true;
      blinkStarted = millis();
      Serial.println("Startup: ring filled; beginning two blink cycles");
    }
    return;
  }

  // Phase 2: two 300 ms cosine cycles for a smooth blink in/out motion.
  const unsigned long blinkDuration = 600;
  const unsigned long elapsed = millis() - blinkStarted;
  if (elapsed >= blinkDuration) {
    // Phase 3: IDLE provides the black target frame and the transition helper
    // fades the completed startup ring smoothly to off.
    Serial.println("Startup: blink cycles complete; fading to IDLE");
    setState(STATE_IDLE, STARTUP_FADE_OUT_DURATION_MS);
    return;
  }

  const float cycleProgress = (elapsed % 300) / 300.0f;
  const float intensity = 0.5f + 0.5f * cos(2.0f * PI * cycleProgress);
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, intensity));
  ledRing.show();
}
