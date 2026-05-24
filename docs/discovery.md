# Discovering Your Pool Controller

The ESPHome configuration needs the BLE MAC address of your own pool controller.

The known app surface for this controller is pump and light control only. If your app shows additional equipment, treat that as unverified and do not assume this bridge supports it.

## With A BLE Scanner App

1. Stand near the pool controller.
2. Close the HPOWER phone app so the controller is free to advertise.
3. Open a BLE scanner app such as nRF Connect.
4. Look for a device with a name similar to `TPM-POOL-*` or another pool-related name.
5. Confirm it advertises service UUID `0000ffe0-0000-1000-8000-00805f9b34fb`.
6. Move closer and farther away to confirm the RSSI changes as expected.
7. Copy that device's public MAC address into `pool_mac`.

## Expected GATT Shape

The known HPOWER-style controller exposes:

| Service UUID | Characteristic UUID | Properties |
| --- | --- | --- |
| `0000ffe0-0000-1000-8000-00805f9b34fb` | `0000ffe1-0000-1000-8000-00805f9b34fb` | read, write, write without response, notify |

If your controller does not expose this service and characteristic, this ESPHome configuration probably will not work without protocol changes.
