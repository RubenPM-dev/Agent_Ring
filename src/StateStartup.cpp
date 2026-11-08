#include "StateStartup.h"
#include <Arduino.h>
#include <math.h>
#include "JarvisConfig.h"
#include "JarvisState.h"
#include "LedRing.h"

namespace {
bool fillComplete = false;
unsigned long blinkStarted = 0;
}

// Reset the startup phases so the animation can run again from the beginning.
void resetStartup() {
  fillComplete = false;
  blinkStarted = 0;
}

void updateStartup() {
  // Phase 1: add one more LED to the ring every frame until it is full.
  const unsigned long frameTime = 25;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();

  if (!fillComplete) {
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
      Serial.print("Startup: ring filled at ");
      Serial.print(blinkStarted);
      Serial.println(" ms; beginning two blink cycles");
    }
    return;
  }

  // Phase 2: use two 300 ms cosine cycles for smooth blink in/out motion.
  const unsigned long blinkDuration = 600;
  const unsigned long elapsed = millis() - blinkStarted;
  if (elapsed >= blinkDuration) {
    // Phase 3: switch to IDLE, which provides the black target frame. The
    // transition helper fades the completed startup ring down to that target.
    Serial.print("Startup: blink cycles complete at ");
    Serial.print(millis());
    Serial.println(" ms; fading to IDLE");
    setState(STATE_IDLE, STARTUP_FADE_OUT_DURATION_MS);
    return;
  }

  const float cycleProgress = (elapsed % 300) / 300.0f;
  const float intensity = 0.5f + 0.5f * cos(2.0f * PI * cycleProgress);
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, intensity));
  ledRing.show();
}
