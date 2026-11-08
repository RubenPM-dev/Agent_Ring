#include "StateIdle.h"
#include "LedRing.h"

void updateIdle() {
  // IDLE is the off state: render an all-black target frame. State transitions
  // can crossfade into this frame instead of switching off instantly.
  ledRing.clear();
  ledRing.show();
}

