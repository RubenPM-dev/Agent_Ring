#include "SerialCommands.h"
#include "JarvisConfig.h"
#include "states/JarvisState.h"

namespace {
char commandBuffer[80];
int commandPosition = 0;
}

static void printStatus() {
  Serial.println();
  Serial.println("----- JARVIS STATUS -----");
  Serial.print("State: "); Serial.println(getStateName());
  Serial.print("Brightness: "); Serial.print(userBrightness); Serial.println("%");
  Serial.print("Color: "); Serial.print(baseR); Serial.print(",");
  Serial.print(baseG); Serial.print(","); Serial.println(baseB);
  Serial.print("Voice level: "); Serial.println(voiceLevel);
  Serial.print("Animation step: "); Serial.println(animationStep);
  Serial.println("-------------------------");
  Serial.println();
}

static void processCommand(String command) {
  command.trim();
  if (command.length() == 0) return;
  command.toUpperCase();
  Serial.print("Received: "); Serial.println(command);

  if (command.startsWith("LEVEL:")) {
    voiceLevel = constrain(command.substring(6).toInt(), 0, 255);
    lastVoiceLevelUpdate = millis();
    return;
  }

  if (command.startsWith("BRIGHTNESS:")) {
    String valueString = command.substring(11);
    valueString.trim();
    if (valueString.startsWith("+") || valueString.startsWith("-")) userBrightness += valueString.toInt();
    else userBrightness = valueString.toInt();
    userBrightness = constrain(userBrightness, MIN_BRIGHTNESS, MAX_BRIGHTNESS);
    Serial.print("Brightness = "); Serial.print(userBrightness); Serial.println("%");
    return;
  }

  if (command.startsWith("COLOR:")) {
    String values = command.substring(6);
    const int comma1 = values.indexOf(',');
    const int comma2 = values.indexOf(',', comma1 + 1);
    if (comma1 > 0 && comma2 > comma1) {
      baseR = constrain(values.substring(0, comma1).toInt(), 0, 255);
      baseG = constrain(values.substring(comma1 + 1, comma2).toInt(), 0, 255);
      baseB = constrain(values.substring(comma2 + 1).toInt(), 0, 255);
      Serial.print("Color = "); Serial.print(baseR); Serial.print(",");
      Serial.print(baseG); Serial.print(","); Serial.println(baseB);
    } else Serial.println("Invalid COLOR format");
    return;
  }

  if (command == "STATUS") {
    printStatus();
    return;
  }

  if (command == "STARTUP") setState(STATE_STARTUP);
  else if (command == "IDLE") setState(STATE_IDLE);
  else if (command == "LISTENING") setState(STATE_LISTENING);
  else if (command == "LISTENING_DUO_PULSATING") setState(STATE_LISTENING_DUO_PULSATING);
  else if (command == "LISTENING_QUAD_PULSATING") setState(STATE_LISTENING_QUAD_PULSATING);
  else if (command == "THINKING") setState(STATE_THINKING);
  else if (command == "THINKING_PULSATING") setState(STATE_THINKING_PULSATING);
  else if (command == "THINKING_DUO") setState(STATE_THINKING_DUO);
  else if (command == "SPEAKING") setState(STATE_SPEAKING);
  else if (command == "SPEAKING_QUAD_PULSATING") setState(STATE_SPEAKING_QUAD_PULSATING);
  else if (command == "ERROR") setState(STATE_ERROR);
  else if (command == "STATE_SPEAKING_DEBUG") {
    setState(STATE_SPEAKING_DEBUG);
    speakingDebugStart = millis();
    smoothedVoiceLevel = 0;
    Serial.println("Speaking debug simulation started");
  } else Serial.println("Unknown command");
}

void beginSerialCommands() {
  Serial.begin(115200);
  // Never let a disconnected USB serial monitor block firmware animations or
  // state transitions while diagnostic output is being written.
  Serial.setTxTimeoutMs(0);
  delay(500);
  Serial.println();
  Serial.println("JARVIS LED controller ready");
  Serial.println("---------------------------");
  Serial.println("Commands:");
  Serial.println("STARTUP");
  Serial.println("IDLE");
  Serial.println("LISTENING");
  Serial.println("LISTENING_DUO_PULSATING");
  Serial.println("LISTENING_QUAD_PULSATING");
  Serial.println("THINKING");
  Serial.println("THINKING_PULSATING");
  Serial.println("THINKING_DUO");
  Serial.println("SPEAKING");
  Serial.println("SPEAKING_QUAD_PULSATING");
  Serial.println("ERROR");
  Serial.println("STATE_SPEAKING_DEBUG");
  Serial.println("LEVEL:0-255");
  Serial.println("BRIGHTNESS:20-100");
  Serial.println("BRIGHTNESS:+10");
  Serial.println("BRIGHTNESS:-10");
  Serial.println("COLOR:R,G,B");
  Serial.println("STATUS");
  Serial.println();
}

void updateSerialCommands() {
  while (Serial.available() > 0) {
    const char c = Serial.read();
    if (c == '\r') continue;
    if (c == '\n') {
      commandBuffer[commandPosition] = '\0';
      if (commandPosition > 0) processCommand(commandBuffer);
      commandPosition = 0;
      continue;
    }
    if (commandPosition < (int)sizeof(commandBuffer) - 1) {
      commandBuffer[commandPosition] = c;
      commandPosition++;
    }
  }
}

