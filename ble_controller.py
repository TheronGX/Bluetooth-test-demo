import asyncio
from bleak import BleakClient, BleakScanner

# Must match the name defined in your ESP32-S3 main.cpp
DEVICE_NAME = "ESP32_S3_LED_Control"
CHARACTERISTIC_UUID = "beb5483e-36e1-4688-b7f5-ea07361b26a8"

async def main():
    print(f"Scanning for '{DEVICE_NAME}'...")
    
    # Locate the ESP32-S3 advertising on Bluetooth
    device = await BleakScanner.find_device_by_name(DEVICE_NAME, timeout=10.0)

    if not device:
        print(f"Error: Could not find '{DEVICE_NAME}'.")
        print("Check that the board is powered on and running the updated C++ code.")
        return

    print(f"Found device! Connecting to {device.address}...")

    async with BleakClient(device) as client:
        print(f"Connected to {DEVICE_NAME}\n")
        print("--- Controls ---")
        print("Type '1' + Enter -> Turn RGB LED Green")
        print("Type '0' + Enter -> Turn RGB LED Off")
        print("Type 'q' + Enter -> Quit\n")

        loop = asyncio.get_event_loop()
        
        while True:
            # Capture keyboard input asynchronously
            user_input = await loop.run_in_executor(None, input, "Key press > ")
            cmd = user_input.strip()

            if cmd == '1':
                # Write byte string '1' to the characteristic
                await client.write_gatt_char(CHARACTERISTIC_UUID, b'1')
                print("-> Command Sent: LED ON")
            elif cmd == '0':
                # Write byte string '0' to the characteristic
                await client.write_gatt_char(CHARACTERISTIC_UUID, b'0')
                print("-> Command Sent: LED OFF")
            elif cmd.lower() == 'q':
                print("Disconnecting and exiting...")
                break
            else:
                print("Invalid input. Use '1' for ON, '0' for OFF, or 'q' to quit.")

if __name__ == "__main__":
    try:
        asyncio.run(main())
    except KeyboardInterrupt:
        print("\nProgram terminated.")