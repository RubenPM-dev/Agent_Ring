#include "StateError.h"
#include <Arduino.h>
#include <math.h>
#include "../../JarvisConfig.h"
#include "../../LedRing.h"

void updateError() {
  // ERROR uses a red breathing pulse to make a fault visually distinct.
  const unsigned long frameTime = 15;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();
  const float wave = (sin(animationStep * 0.07f) + 1.0f) / 2.0f;
  const float intensity = 0.10f + wave * 0.90f;
  ledRing.fill(ledRing.scaledColor(255, 0, 0, intensity));
  ledRing.show();
  animationStep++;
}

