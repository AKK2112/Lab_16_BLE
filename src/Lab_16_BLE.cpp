/******************************************************/
//       THIS IS A GENERATED FILE - DO NOT EDIT       //
/******************************************************/

#include "Particle.h"
#line 1 "d:/Labs/Lab_16_BLE/src/Lab_16_BLE.ino"
#include "oled-wing-adafruit.h"

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context);
void setup();
void loop();
#line 3 "d:/Labs/Lab_16_BLE/src/Lab_16_BLE.ino"
SYSTEM_THREAD(ENABLED);
OledWingAdafruit display;

const size_t UART_TX_BUF_SIZE = 20;

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL);

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)
{
  for (size_t i = 0; i < len; i++)
  {
    if (data[0] == '1')
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Never gonna give you up, Never gonna let you down...");
      display.display();
    }
    if (data[0] == '0')
    {
      RGB.control(true);
      RGB.color(255, 255, 0);
      RGB.brightness(255);
      BLE.disconnect();
    }
    Serial.write(data[i]);
    
  }
}

void setup()
{
  display.setup();
  display.clearDisplay();
  display.display();
  BLE.on();
  BLE.addCharacteristic(txCharacteristic);
  BLE.addCharacteristic(rxCharacteristic);

  BleAdvertisingData data;
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
}

void loop()
{
  display.loop();
  if (BLE.connected())
  {
    uint8_t txBuf[UART_TX_BUF_SIZE];
    String message = "Hello!\n";
    message.toCharArray((char *)txBuf, message.length() + 1);
    txCharacteristic.setValue(txBuf, message.length() + 1);
    RGB.control(true);
    RGB.color(0, 0, 255);
    RGB.brightness(255);
  }
  else
  {
    RGB.control(true);
    RGB.color(255, 255, 0);
    RGB.brightness(255);
  }
  delay(1000);
  
}
