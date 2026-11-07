#include "StateListening.h"
#include <Arduino.h>
#include <math.h>
#include "JarvisConfig.h"
#include "LedRing.h"

void updateListening() {
  const unsigned long frameTime = 10;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();
  const float wave = (sin(animationStep * 0.14f) + 1.0f) / 2.0f;
  const float intensity = 0.45f + wave * 0.55f;
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, intensity));
  ledRing.show();
  animationStep++;
}

