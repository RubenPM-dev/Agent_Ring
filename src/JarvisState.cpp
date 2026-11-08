#include "JarvisState.h"
#include <Arduino.h>
#include "JarvisConfig.h"
#include "LedRing.h"

JarvisState currentState = STATE_STARTUP;

void setState(JarvisState newState) {
  const bool changingState = newState != currentState;
  if (changingState) ledRing.startTransition(STATE_TRANSITION_DURATION_MS);

  currentState = newState;
  animationStep = 0;
  lastFrame = 0;
  if (newState == STATE_SPEAKING || newState == STATE_SPEAKING_DEBUG) smoothedVoiceLevel = 0;
  if (!changingState) {
    ledRing.clear();
    ledRing.show();
  }
  Serial.print("State -> ");
  Serial.println(getStateName());
}

const char* getStateName() {
  switch (currentState) {
    case STATE_STARTUP: return "STARTUP";
    case STATE_IDLE: return "IDLE";
    case STATE_LISTENING: return "LISTENING";
    case STATE_THINKING: return "THINKING";
    case STATE_THINKING_PULSATING: return "THINKING_PULSATING";
    case STATE_SPEAKING: return "SPEAKING";
    case STATE_ERROR: return "ERROR";
    case STATE_SPEAKING_DEBUG: return "STATE_SPEAKING_DEBUG";
  }
  return "UNKNOWN";
}

