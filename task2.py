import asyncio
import struct
import numpy as np
import matplotlib.pyplot as plt
from bleak import BleakClient

# UUIDs must match the Arduino code
DEVICE_NAME = "Nano33BLE"
SERVICE_UUID = "19B10000-E8F2-537E-4F6C-D104768A1214"
CHAR_UUID = "19B10001-E8F2-537E-4F6C-D104768A1214"

# Global storage for accelerometer data
data_buffer = {"x": [], "y": [], "z": []}

async def notification_handler(sender, data):
    # Convert the received byte data to float values
    ax, ay, az = struct.unpack('fff', data)
    data_buffer["x"].append(ax)
    data_buffer["y"].append(ay)
    data_buffer["z"].append(az)
    print(f"Accel: x={ax:.2f}, y={ay:.2f}, z={az:.2f}")

async def run_ble():
    async with BleakClient(DEVICE_NAME) as client:
        print(f"Connected to {DEVICE_NAME}")

        # Subscribe to accelerometer notifications
        await client.start_notify(CHAR_UUID, notification_handler)

        # Collect data for 10 seconds
        await asyncio.sleep(10)
        await client.stop_notify(CHAR_UUID)

        print("Stopped notifications.")

# Run the BLE loop
asyncio.run(run_ble())

# Plot the results
plt.figure()
plt.plot(data_buffer["x"], label="X-axis")
plt.plot(data_buffer["y"], label="Y-axis")
plt.plot(data_buffer["z"], label="Z-axis")
plt.xlabel("Samples")
plt.ylabel("Acceleration (g)")
plt.legend()
plt.title("Nano 33 BLE Accelerometer Data")
plt.show()
