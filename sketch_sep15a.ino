#include "JarvisState.h"
#include "LedRing.h"
#include "SerialCommands.h"
#include "StateError.h"
#include "StateIdle.h"
#include "StateListening.h"
#include "StateOff.h"
#include "StateSpeaking.h"
#include "StateSpeakingDebug.h"
#include "StateThinking.h"

void setup() {
  beginSerialCommands();
  ledRing.begin();
  ledRing.clear();
  ledRing.show();
}

void loop() {
  updateSerialCommands();
  switch (currentState) {
    case STATE_IDLE: updateIdle(); break;
    case STATE_LISTENING: updateListening(); break;
    case STATE_THINKING: updateThinking(); break;
    case STATE_SPEAKING: updateSpeaking(); break;
    case STATE_ERROR: updateError(); break;
    case STATE_OFF: updateOff(); break;
    case STATE_SPEAKING_DEBUG: updateSpeakingDebug(); break;
  }
}
