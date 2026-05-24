# Protocol Notes

The HPOWER controller uses a vendor-style BLE service and characteristic:

| Name | UUID |
| --- | --- |
| Service | `0000ffe0-0000-1000-8000-00805f9b34fb` |
| Characteristic | `0000ffe1-0000-1000-8000-00805f9b34fb` |

The HPOWER and Toscano `tpool` / `tpool-b` apps appear to belong to the same controller family: public app listings identify Toscano Linea Electronica SL as the HPOWER app developer, and the HPOWER and `tpool` app descriptions cover the same BLE-pool / TPM-Pool pump and light workflow. The notes below are based on HPOWER captures, but `tpool` documentation may help explain the same protocol family.

Commands are five-byte writes to `FFE1`.

| Command | Payload |
| --- | --- |
| Status poll | `65 6F DE 00 00` |
| Pump on | `65 6F DE 01 00` |
| Pump off | `65 6F DE 02 00` |
| Pump schedule mode | `65 6F DE 03 00` |
| Light on | `65 6F DE 04 00` |
| Light off | `65 6F DE 05 00` |
| Light schedule mode | `65 6F DE 06 00` |

The observed app surface is limited to pump and light control. It does not expose controls for heaters, chlorinators, valves, or other pool equipment in this setup.

The app has schedule-setting screens, but editing schedules currently crashes on iPhone in the observed setup. The payloads above only switch pump or light back to schedule mode; they do not create or modify schedule times. Schedule editing is future work.

Status notifications are parsed as 22-byte frames beginning with:

```text
65 6F DE
```

The current ESPHome bridge decodes the final byte in each 22-byte status frame:

| Bit mask | Meaning |
| --- | --- |
| `0x80` | Pump running |
| `0x40` | Pump mode on |
| `0x20` | Pump mode off |
| `0x10` | Light on |
| `0x08` | Light mode on |
| `0x04` | Light mode off |

If neither pump mode bit is set, the bridge reports pump mode as `Schedule`. If neither light mode bit is set, it reports light mode as `Schedule`.

Keep `Last Frame` enabled while validating new hardware so you can compare decoded state with the raw BLE response.
