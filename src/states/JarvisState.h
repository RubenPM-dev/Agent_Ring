#pragma once

enum JarvisState {
  STATE_STARTUP,
  STATE_IDLE,
  STATE_LISTENING,
  STATE_LISTENING_DUO_PULSATING,
  STATE_LISTENING_QUAD_PULSATING,
  STATE_THINKING,
  STATE_THINKING_PULSATING,
  STATE_THINKING_DUO,
  STATE_SPEAKING,
  STATE_SPEAKING_QUAD_PULSATING,
  STATE_ERROR,
  STATE_SPEAKING_DEBUG
};

extern JarvisState currentState;
const char* getStateName();
void setState(JarvisState newState);
void setState(JarvisState newState, unsigned long transitionDurationMs);

