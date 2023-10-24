#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h> //Library to use BLE as server
#include <BLE2902.h> 

#include "DHT.h"
#define DHTTYPE DHT11
#define DHTPIN 15 
DHT dht(DHTPIN, DHTTYPE); 
bool _BLEClientConnected = false;

#define BatteryService BLEUUID((uint16_t)0x180F) 
BLECharacteristic BatteryLevelCharacteristic(BLEUUID((uint16_t)0x2A19), BLECharacteristic::PROPERTY_READ  | BLECharacteristic::PROPERTY_WRITE  | BLECharacteristic::PROPERTY_NOTIFY);
BLEDescriptor BatteryLevelDescriptor(BLEUUID((uint16_t)0x2901));

class MyServerCallbacks : public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      _BLEClientConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      _BLEClientConnected = false;
    }
};

/* ###############################################################  CALL back to receive data from Phone */
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"

class MyCallbacks: public BLECharacteristicCallbacks {

    void onWrite(BLECharacteristic *pCharacteristic) {
      std::string rxValue = pCharacteristic->getValue();
      Serial.println(rxValue[0]);
 
      if (rxValue.length() > 0) {
        Serial.println("*********");
        Serial.print("Received Value: ");
 
        for (int i = 0; i < rxValue.length(); i++) {
          Serial.print(rxValue[i]);
        }
        Serial.println();
        Serial.println("*********");
      }
 
    }
};
/* ############################################################### */

void InitBLE() {
  BLEDevice::init("BLE Battery");
  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pBattery = pServer->createService(BatteryService);

  pBattery->addCharacteristic(&BatteryLevelCharacteristic);
  BatteryLevelDescriptor.setValue("Percentage 0 - 100");
  BatteryLevelCharacteristic.addDescriptor(&BatteryLevelDescriptor);
  BatteryLevelCharacteristic.addDescriptor(new BLE2902());


  /* ###############################################################  define callback */
  BLECharacteristic *pWriteCharacteristic = pBattery->createCharacteristic(
                                         CHARACTERISTIC_UUID_RX,
                                         BLECharacteristic::PROPERTY_WRITE
                                       );
 
  pWriteCharacteristic->setCallbacks(new MyCallbacks());
  /* ############################################################### */
  
  pServer->getAdvertising()->addServiceUUID(BatteryService);

  pBattery->start();
  // Start advertising
  pServer->getAdvertising()->start();
}

void setup() {
  Serial.begin(115200);
  Serial.println(F("DHT11!"));
  Serial.println("Battery Level Indicator - BLE");
  dht.begin();
  InitBLE();
}
int intValue = 42;
float floatValue = 3.14;
String message = "Hello, world!";
char charArray[] = "Arduino";
void loop() {
 delay(2000);
 float h = dht.readHumidity();
 float t = dht.readTemperature();
 String temp="Temperature: "+String(t)+"*C\n";
 BLE_print(temp);
// BLE_print("Temperature: ");
// BLE_print(t);
// BLE_print("*C\n");
 Value1(t);
 String humi ="Humidity: "+String(h)+"%\n";
// BLE_print("Humidity: ");
// BLE_print(h);
// BLE_print("%\n");
BLE_print(humi);
 Value2(h);
}
template <typename T>
void BLE_print(T myVar) {
  String value = String(myVar);
  uint8_t data[value.length()];
  for (size_t i = 0; i < value.length(); i++) {
      data[i] = static_cast<uint8_t>(value.charAt(i));
  }
  BatteryLevelCharacteristic.setValue(data, value.length());
  BatteryLevelCharacteristic.notify();
  Serial.print(myVar);
}
template <typename T>
void Value1(T myVar) {
  String value = "1"+String(myVar);
  uint8_t data[value.length()];
  for (size_t i = 0; i < value.length(); i++) {
      data[i] = static_cast<uint8_t>(value.charAt(i));
  }
  BatteryLevelCharacteristic.setValue(data, value.length());
  BatteryLevelCharacteristic.notify();
  //Serial.print(myVar);
}
template <typename T>
void Value2(T myVar) {
  String value = "2"+String(myVar);
  uint8_t data[value.length()];
  for (size_t i = 0; i < value.length(); i++) {
      data[i] = static_cast<uint8_t>(value.charAt(i));
  }
  BatteryLevelCharacteristic.setValue(data, value.length());
  BatteryLevelCharacteristic.notify();
  //Serial.print(myVar);
}
