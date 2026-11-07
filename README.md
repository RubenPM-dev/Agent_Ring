# J.A.R.V.I.S. ESP32-S3 LED Ring

Modular Arduino firmware for the J.A.R.V.I.S. LED ring controller.

## Hardware

- Board: ESP32-S3
- LED data GPIO: `4`
- LEDs: `72` NeoPixels
- Pixel format: `NEO_GRB + NEO_KHZ800`
- Serial speed: `115200`
- Library: Adafruit NeoPixel

Brightness is scaled in firmware and constrained to `20–100%`. The `OFF` state is handled independently.

## Build and upload

The project remains an Arduino sketch and does not use PlatformIO.

With Arduino CLI:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32s3 .
arduino-cli upload --fqbn esp32:esp32:esp32s3 --port /dev/cu.usbmodem101 .
arduino-cli monitor --port /dev/cu.usbmodem101 --config baudrate=115200
```

Update the serial port if the board appears under a different device name.

In VS Code, use **Terminal → Run Task**:

- **JARVIS: Verify / Compile**
- **JARVIS: Verify / Compile (Verbose)**
- **JARVIS: Upload**
- **JARVIS: Verify + Upload**
- **JARVIS: Serial Monitor**
- **JARVIS: Debug Session**

The VS Code defaults are configured in [.vscode/settings.json](.vscode/settings.json). See [DEBUGGING.md](DEBUGGING.md) for details.

## Serial commands

Send each command followed by a newline:

```text
IDLE
LISTENING
THINKING
SPEAKING
ERROR
OFF
STATE_SPEAKING_DEBUG
LEVEL:0-255
BRIGHTNESS:20-100
BRIGHTNESS:+10
BRIGHTNESS:-10
COLOR:R,G,B
STATUS
```

`STATE_SPEAKING_DEBUG` runs the simulated voice waveform and reuses the normal speaking animation.

## Source layout

- [sketch_sep15a.ino](sketch_sep15a.ino) — setup, loop, and high-level orchestration.
- [JarvisConfig.h/.cpp](JarvisConfig.h) — hardware constants and shared runtime settings.
- [JarvisState.h/.cpp](JarvisState.h) — state enumeration, transitions, and state names.
- [LedRing.h/.cpp](LedRing.h) — NeoPixel hardware access, colour scaling, and brightness handling.
- [SerialCommands.h/.cpp](SerialCommands.h) — non-blocking serial input and command processing.
- `State*.h/.cpp` — independent implementations for each LED state.

## Compatibility

The firmware is intended for the ESP32 Arduino Core 3.3.x and Adafruit NeoPixel library. The current implementation has been compile-verified with ESP32 Core 3.3.12 and Adafruit NeoPixel 1.15.5.
