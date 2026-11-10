#pragma once

#include <Arduino.h>

constexpr uint8_t LED_PIN = 4;
constexpr uint16_t LED_COUNT = 72;
constexpr int MIN_BRIGHTNESS = 20;
constexpr int MAX_BRIGHTNESS = 100;
constexpr unsigned long STATE_TRANSITION_DURATION_MS = 300;
constexpr unsigned long STARTUP_FADE_OUT_DURATION_MS = 350;

extern int userBrightness;
extern int baseR;
extern int baseG;
extern int baseB;
extern float voiceLevel;
extern float smoothedVoiceLevel;
extern unsigned long lastVoiceLevelUpdate;
extern unsigned long speakingDebugStart;
extern unsigned long lastFrame;
extern uint32_t animationStep;
