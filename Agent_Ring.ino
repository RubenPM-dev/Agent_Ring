#include "src/JarvisState.h"
#include "src/LedRing.h"
#include "src/SerialCommands.h"
#include "src/StateError.h"
#include "src/StateIdle.h"
#include "src/StateListening.h"
#include "src/StateSpeaking.h"
#include "src/StateSpeakingDebug.h"
#include "src/StateStartup.h"
#include "src/StateThinking.h"

void setup() {
  beginSerialCommands();
  ledRing.begin();
  resetStartup();
  ledRing.clear();
  ledRing.show();
}

void loop() {
  updateSerialCommands();
  switch (currentState) {
    case STATE_STARTUP: updateStartup(); break;
    case STATE_IDLE: updateIdle(); break;
    case STATE_LISTENING: updateListening(); break;
    case STATE_THINKING: updateThinking(); break;
    case STATE_THINKING_PULSATING: updateThinkingPulsating(); break;
    case STATE_THINKING_DUO: updateThinkingDuo(); break;
    case STATE_SPEAKING: updateSpeaking(); break;
    case STATE_ERROR: updateError(); break;
    case STATE_SPEAKING_DEBUG: updateSpeakingDebug(); break;
  }
}
