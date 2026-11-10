# JARVIS LED Ring Interface

This document is the host/AI-facing contract for interacting with the JARVIS LED
Ring firmware.

## Transport

- Interface: USB serial
- Baud rate: `115200`
- Line ending: send `\n` after every command
- Commands are case-insensitive
- The firmware accepts one command per line
- The configured macOS port is stored in `.vscode/settings.json`; it may change
  when the board is reconnected

Example:

```text
THINKING_PULSATING\n
```

## Commands

### State commands

| Command | Effect |
| --- | --- |
| `STARTUP` | Retriggers the fill-and-double-blink startup animation, then transitions to `IDLE`. |
| `IDLE` | Turns the ring off. The current display crossfades to black. |
| `LISTENING` | Shows the selected base colour at the dim listening intensity. |
| `THINKING` | Shows a dim ring with a bright highlight rotating around it. |
| `THINKING_PULSATING` | Shows the rotating Thinking highlight with a breathing brightness pulse. |
| `THINKING_DUO` | Shows two shorter synchronized bright highlights rotating opposite each other around an otherwise dark ring. |
| `SPEAKING` | Animates the ring from the current `LEVEL` value. |
| `ERROR` | Shows a red pulsing error animation. |
| `STATE_SPEAKING_DEBUG` | Runs a simulated speaking waveform without live audio input. |

Every normal state change uses a smooth crossfade of approximately `300 ms`.
The startup animation uses a smooth `350 ms` fade when switching off after its
two blink cycles.

### Brightness

```text
BRIGHTNESS:20
BRIGHTNESS:70
BRIGHTNESS:+10
BRIGHTNESS:-10
```

Brightness is constrained to `20–100%` and affects all colour output.

### Colour

```text
COLOR:<red>,<green>,<blue>
```

Each channel is constrained to `0–255`.

Example:

```text
COLOR:0,220,190
```

The selected base colour is used by `LISTENING`, `THINKING`,
`THINKING_PULSATING`, and `SPEAKING`. `ERROR` remains red and `IDLE` remains off.

### Speaking level

```text
LEVEL:0
LEVEL:128
LEVEL:255
```

Sets the current speaking amplitude from `0–255`. A host should calculate this
from the audio waveform, normally using an RMS or peak value over a short audio
window, normalize it to `0–255`, and send updates around `20–50` times per second.
The host should continue sending `LEVEL:0` during pauses. If updates stop for
approximately `150 ms`, the firmware treats the input as silence and smoothly
decays toward the quiet intensity. The debug speaking state generates its own
simulated level values.

### Status

```text
STATUS
```

Requests the current state, brightness, base colour, voice level, and animation
step.

## Startup sequence

When the board starts, it automatically:

1. Fills the ring one pixel at a time.
2. Performs two smooth blink cycles after the ring is full.
3. Crossfades into `IDLE`.

The startup animation is interrupted safely if a valid state command is received.
It can be retriggered at any time by sending `STARTUP` followed by a newline.

## Responses

For a recognized command, the firmware normally prints:

```text
Received: <COMMAND>
```

For a state command it also prints:

```text
State -> <STATE_NAME>
```

Invalid or unknown input produces one of:

```text
Invalid COLOR format
Unknown command
```

## Recommended AI interaction

1. Open the serial connection at `115200` baud.
2. Send one newline-terminated command at a time.
3. Wait for the acknowledgement or state response before sending another state command.
4. Use `STATUS` when the current state is unknown.
5. Prefer `THINKING_PULSATING` when the Ring should visibly indicate active processing.
6. Use `SPEAKING` with repeated `LEVEL:0–255` updates for live voice activity.

`THINKING_DUO` is the serial command for the paired rotating highlight mode.

## Hardware summary

- Board: ESP32-S3
- LED count: `72`
- LED data pin: GPIO `4`
- Pixel protocol: NeoPixel GRB, `800 kHz`
