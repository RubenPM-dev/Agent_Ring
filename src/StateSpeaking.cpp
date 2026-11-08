#include "StateSpeaking.h"
#include <Arduino.h>
#include <math.h>
#include "JarvisConfig.h"
#include "LedRing.h"

void updateSpeaking() {
  // SPEAKING maps the smoothed voice level to the ring's brightness and motion.
  const unsigned long frameTime = 15;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();
  if (voiceLevel > smoothedVoiceLevel) smoothedVoiceLevel = smoothedVoiceLevel * 0.25f + voiceLevel * 0.75f;
  else smoothedVoiceLevel = smoothedVoiceLevel * 0.80f + voiceLevel * 0.20f;
  const float normalized = smoothedVoiceLevel / 255.0f;
  const float baseIntensity = 0.15f + normalized * (1.0f - 0.15f);
  for (int i = 0; i < LED_COUNT; i++) {
    const float wave = (sin(i * 0.35f + animationStep * 0.22f) + 1.0f) / 2.0f;
    const float movement = wave * 0.20f * normalized;
    const float intensity = constrain(baseIntensity + movement, 0.0f, 1.0f);
    ledRing.setPixelColor(i, ledRing.scaledColor(baseR, baseG, baseB, intensity));
  }
  ledRing.show();
  animationStep++;
}

