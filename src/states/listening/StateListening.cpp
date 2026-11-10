#include "StateListening.h"
#include "../../JarvisConfig.h"
#include "../../LedRing.h"

void updateListening() {
  // LISTENING is a quiet, steady indication using the selected base colour.
  // Match the dimmest background level used by Thinking.
  const float effectIntensity = 0.08f;
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, effectIntensity));
  ledRing.show();
}

