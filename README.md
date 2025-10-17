## What this code does

This repository is a SensESP-based project that exposes ESP32 hardware (sensors, relays, etc.) to a Signal K server. On first boot it starts a WiFi configuration portal (SensESP), prints status to the serial console, and can auto-register with a Signal K server so devices and controls appear in Signal K clients.

## About SensESP and Signal K

- SensESP: lightweight framework that connects microcontroller inputs/outputs to Signal K using JSON-over-HTTP/WS.
- Signal K: an open standard and server for marine data exchange and apps.

## Quick start

- Fork or clone the repo and open it in VS Code or a shell.
- Build and upload to an ESP32 (PlatformIO or PlatformIO Core).
- Open the serial monitor to view boot messages and the device IP.
- On first boot the device hosts a WiFi AP (SSID = device name, password: `thisisfine`) — connect and enter your WiFi and Signal K server details in the configuration portal.
- After configuration the device joins the network and should create new access request with your Signal K server.

## Customize

Edit src/main.cpp to change relays pins and platformio.ini to select the correct ESP32-S3 board/environment.

For full SensESP documentation and examples, see the SensESP docs: https://signalk.org/SensESP/

### Build & deploy (quick setup)

Prerequisites
- VS Code (recommended) or a shell.
- PlatformIO: install the PlatformIO extension in VS Code or install PlatformIO Core (CLI).
- USB drivers for your board (Windows: CP210x, CH340, etc.) and a data USB cable.
- A Signal K server reachable from the device (local or remote) if you want automatic registration.

Clone and open the project
- git clone <repo-url>
- cd into the repository folder and open it in VS Code (or your shell).

Using VS Code + PlatformIO
1. Install the PlatformIO extension if not already installed.
2. Open the project folder. PlatformIO will detect the project.
3. In the bottom left PlatformIO icon -> Project Tasks, expand the environment(s) defined in platformio.ini (e.g. an esp32-s3 env).
4. Run "Build" to compile.
5. Run "Upload" to flash the device (ensure the correct USB port is connected; PlatformIO will usually auto-detect).
6. Use "Monitor" to open the serial console and watch boot output.

Using the PlatformIO CLI
1. From the project root, list valid environments: platformio run -t envlist
2. Build: platformio run -e <env-name>
3. Upload: platformio run -e <env-name> -t upload --upload-port <PORT>
    - Example (Windows): platformio run -e esp32s3 -t upload --upload-port COM3
    - Example (Unix): platformio run -e esp32s3 -t upload --upload-port /dev/ttyUSB0
4. Open the serial console: platformio device monitor --port <PORT> --baud 115200

First boot & configuration
- On first boot the device opens a WiFi config portal (SensESP). It also prints status to the serial console.
- The device AP uses the same name as the device; the default password is thisisfine.
- Connect to the AP, open the configuration page in your browser, and enter your WiFi and Signal K server details.
- After configuration the device will join your network and should register with the Signal K server; the serial console will show the assigned IP and registration messages.

Tips & troubleshooting
- If upload fails, try another USB cable or port. Some cables are power-only.
- If using Windows, ensure the correct COM port is selected in PlatformIO or passed to the CLI.
- Check platformio.ini for the correct board/environment for your ESP32-S3 variant and edit if necessary.
- Use the serial monitor to view boot logs and confirm network/SensESP status.

That's it — build, upload, open the serial monitor, configure WiFi/Signal K via the portal, and your relays should appear to SKipper and the Signal K server.

### SKipper app

SKipper (http://www.skipperapp.net) can control this device. Once the device is online and registered with your Signal K server, add the server (or the device IP) in the SKipper app — the app will discover relays exposed by SensESP and provide on/off control and simple scene grouping.

Here is sample video of operation: (https://social.dytrych.cloud/@skipperapp/114717672196455126)

Quick steps:
- Ensure the device is connected to your network and SensESP is connected to your Signal K server.
- Install and open SKipper, then add your Signal K server or the device IP.
- Use the discovered relay controls to operate the device from your phone or tablet.