# J.A.R.V.I.S. ESP32-S3 LED Ring

Modular Arduino firmware for the J.A.R.V.I.S. LED ring controller.

## Hardware

- Board: ESP32-S3
- LED data GPIO: `4`
- LEDs: `72` NeoPixels
- Pixel format: `NEO_GRB + NEO_KHZ800`
- Serial speed: `115200`
- Library: Adafruit NeoPixel

Brightness is scaled in firmware and constrained to `20–100%`. The ring boots in `IDLE`; `IDLE` clears the LEDs until the host sends another state command.
`LISTENING` uses the same base colour as the other active states and stays steady, capped at 50% brightness (or the selected brightness if lower).

## Build and upload

The project remains an Arduino sketch and does not use PlatformIO.

With Arduino CLI:

```bash
arduino-cli compile --fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc .
arduino-cli upload --fqbn esp32:esp32:esp32s3:USBMode=hwcdc,CDCOnBoot=cdc --port /dev/cu.usbmodem101 .
arduino-cli monitor --port /dev/cu.usbmodem101 --config baudrate=115200
```

USB CDC on boot must be enabled because the server connects through the ESP32-S3's USB serial port and the firmware uses `Serial` for commands and replies. Without it, Arduino maps `Serial` to UART0 instead of the native USB CDC port.

Update the serial port if the board appears under a different device name.

In VS Code, use **Terminal → Run Task**:

- **JARVIS: Verify / Compile**
- **JARVIS: Verify / Compile (Verbose)**
- **JARVIS: Upload**
- **JARVIS: Verify + Upload**
- **JARVIS: Serial Monitor**
- **JARVIS: Debug Session**

The VS Code defaults are configured in [.vscode/settings.json](.vscode/settings.json). See [DEBUGGING.md](DEBUGGING.md) for details.

For the complete host/AI serial protocol, see [RING_INTERFACE.md](RING_INTERFACE.md).

## Serial commands

Send each command followed by a newline:

```text
STARTUP
IDLE
LISTENING
THINKING
THINKING_PULSATING
THINKING_DUO
SPEAKING
ERROR
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

- [Agent_Ring.ino](Agent_Ring.ino) — setup, loop, and high-level orchestration.
- [src/JarvisConfig.h/.cpp](src/JarvisConfig.h) — hardware constants and shared runtime settings.
- [src/JarvisState.h/.cpp](src/JarvisState.h) — state enumeration, transitions, and state names.
- [src/LedRing.h/.cpp](src/LedRing.h) — NeoPixel hardware access, colour scaling, and brightness handling.
- [src/SerialCommands.h/.cpp](src/SerialCommands.h) — non-blocking serial input and command processing.
- `src/State*.h/.cpp` — independent implementations for each LED state.

On boot, the firmware fills the ring, performs two smooth blink cycles, and then
transitions into `IDLE`. `THINKING_PULSATING` adds a breathing brightness pulse to
the rotating Thinking highlight. Sending `STARTUP` retriggers the startup
animation at any time. `THINKING_DUO` shows two shorter synchronized
bright highlights on opposite sides of an otherwise dark ring.

The root sketch is intentionally kept at the project root because Arduino CLI requires the
primary `.ino` file to match the sketch folder name. Implementation modules live in `src/`
so hardware, state, and command code stay separate from the firmware entry point.

## Compatibility

The firmware is intended for the ESP32 Arduino Core 3.3.x and Adafruit NeoPixel library. The current implementation has been compile-verified with ESP32 Core 3.3.12 and Adafruit NeoPixel 1.15.5.
