#!/usr/bin/env bash
set -euo pipefail

device_path="${1:?Usage: release-serial-port.sh /dev/cu.usbmodem...}"
if [[ "$device_path" != /dev/* ]]; then
  printf 'Refusing to signal processes for non-device path: %s\n' "$device_path" >&2
  exit 2
fi
if [[ ! -e "$device_path" ]]; then
  printf 'Configured serial device does not exist: %s\n' "$device_path" >&2
  shopt -s nullglob
  available_ports=(/dev/cu.usbmodem* /dev/cu.usbserial*)
  if (( ${#available_ports[@]} )); then
    printf 'Available USB serial port(s): %s\n' "${available_ports[*]}" >&2
  else
    printf 'No USB serial ports are currently available. Check the cable and board connection.\n' >&2
  fi
  exit 1
fi
if ! command -v lsof >/dev/null 2>&1; then
  printf 'lsof is required to check who owns %s\n' "$device_path" >&2
  exit 2
fi

holders() {
  lsof -t "$device_path" 2>/dev/null || true
}

initial_holders="$(holders | sort -u)"
if [[ -z "$initial_holders" ]]; then
  printf 'Serial port is already free: %s\n' "$device_path"
  exit 0
fi

printf 'Requesting serial port release: %s\n' "$device_path"
while IFS= read -r pid; do
  [[ -z "$pid" ]] || kill -TERM "$pid" 2>/dev/null || true
done <<< "$initial_holders"

for _ in {1..50}; do
  current_holders="$(holders | sort -u)"
  if [[ -z "$current_holders" ]]; then
    printf 'Serial port released: %s\n' "$device_path"
    exit 0
  fi
  sleep 0.1
done

printf 'Force-stopping remaining process(es) holding %s\n' "$device_path" >&2
while IFS= read -r pid; do
  [[ -z "$pid" ]] || kill -KILL "$pid" 2>/dev/null || true
done <<< "$current_holders"

for _ in {1..20}; do
  current_holders="$(holders | sort -u)"
  if [[ -z "$current_holders" ]]; then
    printf 'Serial port released: %s\n' "$device_path"
    exit 0
  fi
  sleep 0.1
done

printf 'Could not release serial port %s; still held by PID(s): %s\n' "$device_path" "$current_holders" >&2
exit 1
