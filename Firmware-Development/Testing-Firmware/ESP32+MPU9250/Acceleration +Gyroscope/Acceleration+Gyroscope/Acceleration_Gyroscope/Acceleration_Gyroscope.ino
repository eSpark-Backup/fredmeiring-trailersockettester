#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif

MPU9250 mySensor;

uint8_t sensorId;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
char val[20],val_1[30];
String A,AX,AY,AZ;
String G,GX,GY,GZ;

String C=",";

BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic_ACC= NULL;
BLECharacteristic* pCharacteristic_GYRO= NULL;


bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID BLEUUID((uint16_t)0xAAA0)
#define CHARACTERISTICS_UUID_ACC  BLEUUID((uint16_t)0xAAA1)
#define CHARACTERISTICS_UUID_GYRO BLEUUID((uint16_t)0xAAA2)
//#define CHARACTERISTICS_UUID_AZ BLEUUID((uint16_t)0xAAA3)


class MyServerCallbacks: public BLEServerCallbacks {
    void onConnect(BLEServer* pServer) {deviceConnected = true;}
    void onDisconnect(BLEServer* pServer) {deviceConnected = false;}
    };


void setup() {
Serial.begin(115200);

#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN); // SDA, SCL
#else
  Wire.begin();
#endif

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
 // mySensor.beginMag();

  // You can set your own offset for mag values
  // mySensor.magXOffset = -50;
  // mySensor.magYOffset = -55;                           
  // mySensor.magZOffset = -10;

  sensorId = mySensor.readId();
 Serial.println("sensorId: " + String(sensorId));

BLEDevice::init("BLE_ACCELERO+GYRO");
  pServer = BLEDevice::createServer();
  
  pServer->setCallbacks(new MyServerCallbacks());
  
   BLEService *pService = pServer->createService(SERVICE_UUID);
   
   pCharacteristic_ACC = pService->createCharacteristic(
                      CHARACTERISTICS_UUID_ACC,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );

                    
   pCharacteristic_GYRO = pService->createCharacteristic(
                      CHARACTERISTICS_UUID_GYRO,
                      BLECharacteristic::PROPERTY_READ   |
                      BLECharacteristic::PROPERTY_WRITE  |
                      BLECharacteristic::PROPERTY_NOTIFY 
                    );


pCharacteristic_ACC->addDescriptor(new BLE2902());
pCharacteristic_GYRO->addDescriptor(new BLE2902());
  pService->start();
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");                    

}

void loop() {
 if (deviceConnected) {
       mySensor.accelUpdate();
        aX = mySensor.accelX();
        aY = mySensor.accelY();
        aZ = mySensor.accelZ();
        
        AX=String(aX)+",";
        AY=String(aY)+",";
        AZ=String(aZ);
        
        A=AX+AY+AZ;
        A.toCharArray(val,20);
        std::string myString((char*)val); 

        mySensor.gyroUpdate();
        gX = mySensor.gyroX();
        gY = mySensor.gyroY();
        gZ = mySensor.gyroZ();

        GX=String(gX)+",";
        GY=String(gY)+",";
        GZ=String(gZ);
       
        G=GX+GY+GZ;
        G.toCharArray(val_1,30);
        std::string myString_1((char*)val_1); 

        pCharacteristic_ACC->setValue(myString);
        pCharacteristic_ACC->notify();
        
        pCharacteristic_GYRO->setValue(myString_1);
        pCharacteristic_GYRO->notify();

        Serial.print("AX: " + String(aX) + " ");
        Serial.print("  AY: " + String(aY) + " ");
        Serial.print("  AZ: " + String(aZ));
        Serial.print("         ");
        Serial.print("GX: " + String(gX) + " ");
        Serial.print("  GY: " + String(gY) + " ");
        Serial.println("  GZ: " + String(gZ)); 
        
        
        
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
