# JARVIS VS Code workflows

Use **Terminal → Run Task** in VS Code and choose one of these tasks:

- **JARVIS: Verify / Compile** — compile-check the sketch.
- **JARVIS: Verify / Compile (Verbose)** — compile with the full Arduino command output.
- **JARVIS: Upload** — compile and upload to the configured ESP32-S3 port.
- **JARVIS: Verify + Upload** — verify first, then upload only if verification succeeds.
- **JARVIS: Serial Monitor** — open the 115200-baud serial monitor.
- **JARVIS: Debug Session** — verbose compile, then start the serial monitor.

The upload workflows release the configured serial port before starting. If an older monitor process still owns the port, stop it with `Ctrl+C` in its terminal, then retry the upload.

You can also open **Run and Debug** and choose a `JARVIS:` launch configuration from [.vscode/launch.json](.vscode/launch.json). Use **JARVIS: Verify Then Upload** when you want VS Code to compile-check the firmware before invoking the upload command.

The defaults are stored in [.vscode/settings.json](./.vscode/settings.json):

- FQBN: `esp32:esp32:esp32s3`
- Port: `/dev/cu.usbmodem1101`
- Baud: `115200`

If the board is assigned a different port, update `jarvis.arduinoPort` in the workspace settings before uploading or monitoring. The debug session exercises the existing serial protocol; source-level JTAG debugging is not configured because it requires an ESP32-S3 debug probe and OpenOCD setup.
