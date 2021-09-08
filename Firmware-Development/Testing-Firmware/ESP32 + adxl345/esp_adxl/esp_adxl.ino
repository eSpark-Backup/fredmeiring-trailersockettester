#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <SparkFun_ADXL345.h>



BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristicx = NULL;
BLECharacteristic* pCharacteristicy = NULL;
BLECharacteristic* pCharacteristicz = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t ax = 0;
uint8_t ay = 0;
uint8_t az = 0;
int  x,y, z;

ADXL345 adxl = ADXL345();

#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUIDx "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUIDy "beb5483f-36e1-4688-b7f5-ea07361b26a8"
#define CHARACTERISTIC_UUIDz "beb5483g-36e1-4688-b7f5-ea07361b26a8"

class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {
      deviceConnected = true;
    };

    void onDisconnect(BLEServer* pServer) {
      deviceConnected = false;
    }
};

void setup() {
  Serial.begin(115200);
  BLEDevice::init("BLE_ADXL");
  adxl.powerOn();
  adxl.setRangeSetting(16);
  adxl.setSpiBit(1);
  adxl.setActivityXYZ(1, 1, 1);
  adxl.InactivityINT(0);
  adxl.ActivityINT(1);
  adxl.FreeFallINT(0);
  adxl.doubleTapINT(0);
  adxl.singleTapINT(0);


  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristicx = pService->createCharacteristic(
                      CHARACTERISTIC_UUIDx,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
 
  pCharacteristicy = pService->createCharacteristic(
                      CHARACTERISTIC_UUIDy,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
  
  pCharacteristicz = pService->createCharacteristic(
                      CHARACTERISTIC_UUIDz,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY
                    );
                                       

  pCharacteristicx->addDescriptor(new BLE2902());
pCharacteristicy->addDescriptor(new BLE2902());
pCharacteristicz->addDescriptor(new BLE2902());
  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
}

void loop() {
    adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
    Serial.print(x);
    Serial.print(", ");
    Serial.print(y);
    Serial.print(", ");
    Serial.println(z);
  if (deviceConnected) {
    
//    adxl.readAccel(&x, &y, &z);         // Read the accelerometer values and store them in variables declared above x,y,z
//    Serial.print(x);
//    Serial.print(", ");
//    Serial.print(y);
//    Serial.print(", ");
//    Serial.println(z);
    ax=x;
    ay=y;
    az=z;
    pCharacteristicx->setValue(&ax, 1);
    pCharacteristicx->notify();
    
    pCharacteristicy->setValue(&ay, 1);
    pCharacteristicy->notify();
    
    pCharacteristicz->setValue(&az, 1);
    pCharacteristicz->notify();
    delay(1000); // bluetooth stack will go into congestion, if too many packets are sent
  }
  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(500); // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising(); // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
    oldDeviceConnected = deviceConnected;
  }










}
