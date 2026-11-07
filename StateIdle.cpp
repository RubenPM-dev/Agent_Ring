#include "StateIdle.h"
#include "JarvisConfig.h"
#include "LedRing.h"

void updateIdle() {
  ledRing.fill(ledRing.scaledColor(baseR, baseG, baseB, 0.15f));
  ledRing.show();
}

