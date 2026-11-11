#include "VoiceOrbit.h"
#include <Arduino.h>
#include <math.h>
#include "../JarvisConfig.h"
#include "../LedRing.h"

void updateVoiceOrbitPulsating(int sectionCount, int red, int green, int blue) {
  const unsigned long frameTime = 20;
  if (millis() - lastFrame < frameTime) return;
  lastFrame = millis();

  const unsigned long voiceUpdateTimeout = 150;
  const float liveVoiceLevel = millis() - lastVoiceLevelUpdate <= voiceUpdateTimeout
    ? voiceLevel
    : 0.0f;
  if (liveVoiceLevel > smoothedVoiceLevel) {
    smoothedVoiceLevel = smoothedVoiceLevel * 0.20f + liveVoiceLevel * 0.80f;
  } else {
    smoothedVoiceLevel = smoothedVoiceLevel * 0.75f + liveVoiceLevel * 0.25f;
  }

  const float normalized = constrain(smoothedVoiceLevel / 255.0f, 0.0f, 1.0f);
  ledRing.clear();
  constexpr float minimumOrbitIntensity = 0.15f;
  const float peakIntensity = minimumOrbitIntensity + normalized * (1.0f - minimumOrbitIntensity);
  const float rotationSpeed = 0.5f;
  const int center = (int)(animationStep * rotationSpeed) % LED_COUNT;
  const float coreRadius = 0.5f;
  const float radius = 3.5f + normalized * 6.5f;

  for (int offset = -10; offset <= 11; offset++) {
    const float distance = fabsf(offset - 0.5f);
    if (distance > radius) continue;
    float fade = 1.0f;
    if (distance > coreRadius) {
      const float trailProgress = (distance - coreRadius) / (radius - coreRadius);
      fade = 0.5f * (1.0f + cos(PI * trailProgress));
      fade = pow(fade, 0.75f);
    }
    const float intensity = fade * peakIntensity;
    for (int section = 0; section < sectionCount; section++) {
      const int sectionCenter = center + section * LED_COUNT / sectionCount;
      const int pixel = (sectionCenter + offset + LED_COUNT) % LED_COUNT;
      ledRing.setPixelColor(pixel, ledRing.scaledColor(red, green, blue, intensity));
    }
  }

  ledRing.show();
  animationStep++;
}
