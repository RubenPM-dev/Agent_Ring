#pragma once

#include <Arduino.h>

constexpr uint8_t LED_PIN = 4;
constexpr uint16_t LED_COUNT = 72;
constexpr int MIN_BRIGHTNESS = 20;
constexpr int MAX_BRIGHTNESS = 100;

// Listening state uses a dedicated green signal colour.
constexpr uint8_t LISTENING_COLOR_R = 0;
constexpr uint8_t LISTENING_COLOR_G = 255;
constexpr uint8_t LISTENING_COLOR_B = 0;

extern int userBrightness;
extern int baseR;
extern int baseG;
extern int baseB;
extern float voiceLevel;
extern float smoothedVoiceLevel;
extern unsigned long speakingDebugStart;
extern unsigned long lastFrame;
extern uint32_t animationStep;
