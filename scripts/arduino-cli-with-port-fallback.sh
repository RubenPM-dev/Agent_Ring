#!/usr/bin/env bash
set -euo pipefail

cli_path="${1:?Usage: arduino-cli-with-port-fallback.sh /path/to/arduino-cli <command> [args...]}"
shift

resolve_port() {
  local preferred="$1"
  if [[ -n "$preferred" && -e "$preferred" ]]; then
    printf '%s\n' "$preferred"
    return 0
  fi

  local candidate
  for candidate in /dev/cu.usbmodem1101 /dev/cu.usbmodem101 /dev/cu.usbserial*; do
    if [[ -e "$candidate" ]]; then
      printf '%s\n' "$candidate"
      return 0
    fi
  done

  return 1
}

args=("$@")
command="${args[0]:-}"
if [[ "$command" != "upload" && "$command" != "monitor" ]]; then
  exec "$cli_path" "${args[@]}"
fi

port_index=-1
configured_port=""
for ((i = 0; i < ${#args[@]}; i++)); do
  if [[ "${args[$i]}" == "--port" ]]; then
    port_index=$i
    configured_port="${args[$((i + 1))]:-}"
    break
  fi
done

resolved_port="$(resolve_port "$configured_port")" || {
  printf 'No USB serial port found. Checked configured port: %s\n' "$configured_port" >&2
  exit 1
}

if [[ "$resolved_port" != "$configured_port" ]]; then
  printf 'Configured port unavailable; using %s\n' "$resolved_port" >&2
fi

if (( port_index >= 0 )); then
  args[$((port_index + 1))]="$resolved_port"
else
  args+=("--port" "$resolved_port")
fi

exec "$cli_path" "${args[@]}"
