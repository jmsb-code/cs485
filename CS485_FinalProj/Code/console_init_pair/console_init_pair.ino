#include "BluetoothSerial.h"

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);

  if (!SerialBT.begin("ESP32_Controller")) {
    while (1);
  }

  Serial.println("ESP32 ready for controller");
}

void loop() {
  if (SerialBT.available()) {
    uint8_t b = SerialBT.read();
    Serial.write(b);   // shows controller data
  }
}