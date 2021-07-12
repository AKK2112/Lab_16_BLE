#include "oled-wing-adafruit.h"

SYSTEM_THREAD(ENABLED);
OledWingAdafruit display;

const size_t UART_TX_BUF_SIZE = 20;

const BleUuid serviceUuid("6E400001-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid rxUuid("6E400002-B5A3-F393-E0A9-E50E24DCCA9E");
const BleUuid txUuid("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");
BleCharacteristic txCharacteristic("tx", BleCharacteristicProperty::NOTIFY, txUuid, serviceUuid);
BleCharacteristic rxCharacteristic("rx", BleCharacteristicProperty::WRITE_WO_RSP, rxUuid, serviceUuid, onDataReceived, NULL); //bluetooth setup stuff

void onDataReceived(const uint8_t *data, size_t len, const BlePeerDevice &peer, void *context)//defines the onDataRecieved function
{
  for (size_t i = 0; i < len; i++)
  {
    if (data[0] == '1')
    {
      display.clearDisplay();
      display.setTextSize(1);
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.println("Never gonna give you up, Never gonna let you down..."); //When a 1 is recieved, it Rick-Rolls the teacher :)
      display.display();
    }
    if (data[0] == '0')
    {
      RGB.control(true); //When a 0 is recieved, it changes the light to yellow and disconnects
      RGB.color(255, 255, 0);
      RGB.brightness(255);
      delay(100);
      BLE.disconnect();
      delay(50);
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

  BleAdvertisingData data;//advertising the bluetooth data
  data.appendServiceUUID(serviceUuid);
  BLE.advertise(&data);
}

void loop()
{
  display.loop();
  if (BLE.connected())
  {
    uint8_t txBuf[UART_TX_BUF_SIZE];
    String message = "Hello!\n"; //sends hello to the UART panel on my phone.  
    message.toCharArray((char *)txBuf, message.length() + 1);
    txCharacteristic.setValue(txBuf, message.length() + 1);
    RGB.control(true);//after it connects, it changes the RGB to blue
    RGB.color(0, 0, 255);
    RGB.brightness(255);
  }
  else
  {
    RGB.control(true);//if the board isnt connected, it is a yellow light
    RGB.color(255, 255, 0);
    RGB.brightness(255);
  }
  delay(1000);
  
}
