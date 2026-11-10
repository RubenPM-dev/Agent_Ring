#pragma once

enum JarvisState {
  STATE_STARTUP,
  STATE_IDLE,
  STATE_LISTENING,
  STATE_THINKING,
  STATE_THINKING_PULSATING,
  STATE_THINKING_DUO,
  STATE_SPEAKING,
  STATE_ERROR,
  STATE_SPEAKING_DEBUG
};

extern JarvisState currentState;
const char* getStateName();
void setState(JarvisState newState);
void setState(JarvisState newState, unsigned long transitionDurationMs);

