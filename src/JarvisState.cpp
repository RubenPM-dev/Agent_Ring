#include "JarvisState.h"
#include <Arduino.h>
#include "JarvisConfig.h"
#include "LedRing.h"

JarvisState currentState = STATE_OFF;

void setState(JarvisState newState) {
  currentState = newState;
  animationStep = 0;
  lastFrame = 0;
  if (newState == STATE_SPEAKING || newState == STATE_SPEAKING_DEBUG) smoothedVoiceLevel = 0;
  ledRing.clear();
  ledRing.show();
  Serial.print("State -> ");
  Serial.println(getStateName());
}

const char* getStateName() {
  switch (currentState) {
    case STATE_IDLE: return "IDLE";
    case STATE_LISTENING: return "LISTENING";
    case STATE_THINKING: return "THINKING";
    case STATE_SPEAKING: return "SPEAKING";
    case STATE_ERROR: return "ERROR";
    case STATE_OFF: return "OFF";
    case STATE_SPEAKING_DEBUG: return "STATE_SPEAKING_DEBUG";
  }
  return "UNKNOWN";
}

