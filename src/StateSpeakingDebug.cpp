#include "StateSpeakingDebug.h"
#include <Arduino.h>
#include "JarvisConfig.h"
#include "StateSpeaking.h"

void updateSpeakingDebug() {
  // Generate a repeatable voice waveform, then reuse the normal SPEAKING renderer.
  static const uint8_t speechPattern[] = {
    0, 0, 18, 70, 145, 95, 35, 8,
    0, 0, 45, 125, 210, 150, 65, 15,
    0, 35, 105, 175, 115, 45, 0, 0,
    65, 155, 230, 135, 55, 10, 0, 0
  };
  const uint32_t sampleInterval = 100;
  const uint32_t sampleCount = sizeof(speechPattern) / sizeof(speechPattern[0]);
  const uint32_t phraseDuration = sampleCount * sampleInterval;
  const uint32_t cycleDuration = 4500;
  const uint32_t elapsed = (millis() - speakingDebugStart) % cycleDuration;
  voiceLevel = 0;
  if (elapsed < phraseDuration) {
    const uint32_t index = elapsed / sampleInterval;
    const uint32_t nextIndex = min(index + 1, sampleCount - 1);
    const float fraction = (elapsed % sampleInterval) / (float)sampleInterval;
    const float currentLevel = speechPattern[index];
    const float nextLevel = speechPattern[nextIndex];
    const float simulatedLevel = currentLevel + (nextLevel - currentLevel) * fraction;
    voiceLevel = (uint8_t)(simulatedLevel + 0.5f);
  }
  updateSpeaking();
}

