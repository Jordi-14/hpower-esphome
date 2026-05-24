# HPOWER Pool ESPHome Bridge

ESPHome configuration for controlling a Hayward HPOWER Bluetooth pool controller from Home Assistant with an ESP32.

This project exposes pump and light controls through Home Assistant, polls the controller over BLE, and publishes the decoded status back as ESPHome entities.

## Safety

Only use this with pool equipment you own and can physically supervise. The observed HPOWER app surface for this controller is limited to pump and light control, but those controls still affect physical equipment. Verify every command on your own hardware before relying on automation.

This project is not affiliated with, endorsed by, or supported by Hayward.

## App Relationship

Hayward's H-Power product page points users to the HPOWER app, but the HPOWER app store listings identify the developer as Toscano Linea Electronica SL. Toscano's `tpool` / `tpool-b` app listings describe the same BLE-pool / TPM-Pool controller workflow and the same pump, light, status, and schedule-related features.

For this project, HPOWER is treated as a Hayward-branded variant in the same Toscano app/protocol family as `tpool`. This repository documents behavior observed through HPOWER, but `tpool` references can be useful when looking for app history, manuals, or related Toscano controller documentation.

## Current Scope

The HPOWER app currently exposes only pump and light control for this controller. This ESPHome bridge follows that scope and does not attempt to control heaters, chlorinators, valves, or any other pool equipment.

The app also has schedule-setting screens, but schedule editing currently crashes on iPhone in the observed setup. This project can switch pump and light back to schedule mode, but full schedule editing is treated as future work.

## What It Provides

- ESPHome BLE client for the HPOWER controller.
- Home Assistant selects for pump mode and light mode.
- Binary sensors for pump running and light on state.
- Diagnostic entities for BLE connection, RSSI, raw status frame, and status byte.
- A manual status poll button.

## Tested Hardware

- Seeed Studio XIAO ESP32-S3.
- HPOWER-compatible Bluetooth pool controller advertising a `FFE0` service and `FFE1` characteristic.

Other ESP32 boards with BLE should work after changing the `esp32.board` value in `esphome/hpower_pool.yaml`.

## Setup

1. Copy the files from `esphome/` into your ESPHome configuration directory.
2. Copy `secrets.example.yaml` to `secrets.yaml` and set your Wi-Fi credentials.
3. Find your pool controller BLE MAC address. See `docs/discovery.md`.
4. Edit `pool_mac` in `hpower_pool.yaml`.
5. Edit `device_name`, `friendly_name`, and `fallback_ssid` if desired.
6. Compile and upload with ESPHome.

```bash
esphome run hpower_pool.yaml
```

After the device is online, add it through Home Assistant's ESPHome integration.

## Configuration

The important substitutions are at the top of `hpower_pool.yaml`:

```yaml
substitutions:
  device_name: hpower-pool
  friendly_name: HPOWER Pool
  pool_mac: "AA:BB:CC:DD:EE:FF"
```

Do not publish your real `secrets.yaml`, Wi-Fi credentials, local IP addresses, packet captures, or raw logs.

## Entities

| Entity | Type | Purpose |
| --- | --- | --- |
| Pump Mode | Select | Sends pump On, Off, or Schedule mode commands. |
| Light Mode | Select | Sends light On, Off, or Schedule mode commands. |
| Pump Running | Binary sensor | Decoded from the latest status frame. |
| Light On | Binary sensor | Decoded from the latest status frame. |
| BLE Connected | Binary sensor | ESP32 connection state to the pool controller. |
| Pool BLE RSSI | Sensor | BLE signal strength. |
| Status Tail | Sensor | Last decoded status byte. |
| Last Frame | Text sensor | Last full decoded status frame in hex. |
| Poll Status | Button | Sends a manual status request. |

## Protocol Notes

The reusable protocol notes are in `docs/protocol.md`. The implementation intentionally sends only known commands and does not expose arbitrary BLE writes.

## References

- [Hayward H-Power product page](https://www.hayward.es/catalog/cuadros-electricos/cuadro-electrico-h-power)
- [HPOWER on Google Play](https://play.google.com/store/apps/details?id=com.ovnisoft.ecopool.hayward)
- [HPOWER on the App Store](https://apps.apple.com/ie/app/hpower/id1436773059)
- [tpool on Google Play](https://play.google.com/store/apps/details?id=com.ovnisoft.ecopool)
- [tpool on the App Store](https://apps.apple.com/es/app/tpool/id1207422645)
