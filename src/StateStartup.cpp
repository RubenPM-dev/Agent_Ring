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

void updateStartup() {
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
    }
    return;
  }

  const unsigned long blinkDuration = 600;
  const unsigned long elapsed = millis() - blinkStarted;
  if (elapsed >= blinkDuration) {
    setState(STATE_IDLE);
    return;
  }

  const float cycleProgress = (elapsed % 300) / 300.0f;
  const float intensity = 0.5f + 0.5f * cos(2.0f * PI * cycleProgress);
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, intensity));
  ledRing.show();
}
