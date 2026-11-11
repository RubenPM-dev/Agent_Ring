#include "StateListening.h"
#include <Arduino.h>
#include "../../JarvisConfig.h"
#include "../../LedRing.h"
#include "../VoiceOrbit.h"

void updateListening() {
  // Reflect the host's live microphone level while keeping LISTENING dimmer
  // than SPEAKING. A stale LEVEL value decays to the quiet 8% floor.
  const unsigned long frameTime = 15;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();

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
  const float intensity = 0.08f + normalized * 0.42f;
  ledRing.fill(ledRing.scaledColor(0, 255, 0, intensity));
  ledRing.show();
  animationStep++;
}

void updateListeningDuoPulsating() { updateVoiceOrbitPulsating(2, 0, 255, 0); }
void updateListeningQuadPulsating() { updateVoiceOrbitPulsating(4, 0, 255, 0); }

