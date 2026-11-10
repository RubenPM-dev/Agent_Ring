#include "JarvisState.h"
#include <Arduino.h>
#include "../JarvisConfig.h"
#include "../LedRing.h"
#include "startup/StateStartup.h"

JarvisState currentState = STATE_STARTUP;

void setState(JarvisState newState) {
  setState(newState, STATE_TRANSITION_DURATION_MS);
}

void setState(JarvisState newState, unsigned long transitionDurationMs) {
  const bool restartingStartup = newState == STATE_STARTUP;
  const bool changingState = newState != currentState;
  if (changingState || restartingStartup) ledRing.startTransition(transitionDurationMs);
  if (restartingStartup) resetStartup();

  currentState = newState;
  animationStep = 0;
  lastFrame = 0;
  if (newState == STATE_SPEAKING || newState == STATE_SPEAKING_DEBUG) smoothedVoiceLevel = 0;
  if (changingState && newState == STATE_IDLE) {
    // Establish the black target frame immediately; LedRing blends it from the
    // captured frame over the requested transition duration.
    ledRing.clear();
    ledRing.show();
  } else if (!changingState && !restartingStartup) {
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
    case STATE_THINKING_DUO: return "THINKING_DUO";
    case STATE_SPEAKING: return "SPEAKING";
    case STATE_ERROR: return "ERROR";
    case STATE_SPEAKING_DEBUG: return "STATE_SPEAKING_DEBUG";
  }
  return "UNKNOWN";
}

