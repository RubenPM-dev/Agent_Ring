#include "JarvisConfig.h"

int userBrightness = 100;
int baseR = 0;
int baseG = 220;
int baseB = 190;
float voiceLevel = 0.0f;
float smoothedVoiceLevel = 0.0f;
unsigned long lastVoiceLevelUpdate = 0;
unsigned long speakingDebugStart = 0;
unsigned long lastFrame = 0;
uint32_t animationStep = 0;

