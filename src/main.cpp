#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>

// Onboard WS2812 RGB LED pin on ESP32-S3-DevKitC-1
constexpr gpio_num_t LED_RGB = GPIO_NUM_38;

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

class LEDCallbacks: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pCharacteristic) override {
        String value = pCharacteristic->getValue().c_str();
        if (value.length() > 0) {
            if (value[0] == '1') {
                // Turn RGB LED ON (Green: Red=0, Green=255, Blue=0)
                neopixelWrite(LED_RGB, 0, 6, 150);
                Serial.println("BLE Command Received: LED ON (Green)");
            } else if (value[0] == '0') {
                // Turn RGB LED OFF (Red=0, Green=0, Blue=0)
                neopixelWrite(LED_RGB, 0, 0, 0);
                Serial.println("BLE Command Received: LED OFF");
            }
        }
    }
};

void setup() {
  Serial.begin(115200);
  
  // Clear RGB LED state on boot
  neopixelWrite(LED_RGB, 0, 0, 0);

  // Initialize BLE
  BLEDevice::init("ESP32_S3_LED_Control");
  BLEServer *pServer = BLEDevice::createServer();
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create Write Characteristic
  BLECharacteristic *pCharacteristic = pService->createCharacteristic(
                                          CHARACTERISTIC_UUID,
                                          BLECharacteristic::PROPERTY_WRITE
                                        );

  pCharacteristic->setCallbacks(new LEDCallbacks());
  pService->start();

  // Start Advertising
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->start();

  Serial.println("BLE Ready on ESP32-S3. Waiting for commands...");
}

void loop() {
  delay(1000);
}


/* LED Blink Test Code
void setup() {
  Serial.begin(115200);
  delay(2000); // Allow time for Serial to initialize
  Serial.println("ESP32-S3 BLE Test Demo");
  Serial.println("[TEST] LED Blink");
}

void loop() {
  neopixelWrite(LED_RGB, 255, 0, 0); // Red
  Serial.println("LED ON (Red)");
  delay(1000);
  neopixelWrite(LED_RGB, 0, 255, 0); // Green
  Serial.println("LED ON (Green)");
  delay(1000);
}
  */