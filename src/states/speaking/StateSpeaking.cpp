#include "StateSpeaking.h"
#include <Arduino.h>
#include <math.h>
#include "../../JarvisConfig.h"
#include "../JarvisState.h"
#include "../../LedRing.h"

void updateSpeaking() {
  // SPEAKING maps the live audio envelope to the ring's brightness and motion.
  const unsigned long frameTime = 15;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();

  // A host that stops sending LEVEL updates is treated as silence after the
  // timeout, preventing the last spoken value from remaining lit indefinitely.
  const unsigned long voiceUpdateTimeout = 150;
  const float liveVoiceLevel = millis() - lastVoiceLevelUpdate <= voiceUpdateTimeout
    ? voiceLevel
    : 0.0f;
  if (liveVoiceLevel > smoothedVoiceLevel) {
    smoothedVoiceLevel = smoothedVoiceLevel * 0.25f + liveVoiceLevel * 0.75f;
  } else {
    smoothedVoiceLevel = smoothedVoiceLevel * 0.80f + liveVoiceLevel * 0.20f;
  }
  const float normalized = smoothedVoiceLevel / 255.0f;
  const float quietIntensity = 0.08f;
  const float baseIntensity = quietIntensity + normalized * (1.0f - quietIntensity);
  for (int i = 0; i < LED_COUNT; i++) {
    const float wave = (sin(i * 0.35f + animationStep * 0.22f) + 1.0f) / 2.0f;
    const float movement = wave * 0.20f * normalized;
    const float intensity = constrain(baseIntensity + movement, 0.0f, 1.0f);
    ledRing.setPixelColor(i, ledRing.scaledColor(baseR, baseG, baseB, intensity));
  }
  ledRing.show();
  animationStep++;
}

