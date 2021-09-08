#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <Wire.h>
#include <Adafruit_ADS1015.h>

Adafruit_ADS1115 ads;

BLEServer *pServer = NULL;
BLECharacteristic * pTxCharacteristic;

bool deviceConnected = false;
bool oldDeviceConnected = false;

int breakSig = 0;
uint32_t result = 0;
float input = 0;

uint32_t value =0;
bool flag = true;
const int ESP_RT = 27;
const int ESP_LT = 25;
const int ESP_FOG = 26;
const int ESP_SL = 18;
const int ESP_PL = 5;
const int ESP_PR = 19;
const int ESP_REV = 17;
const int ESP_IGNSIG = 4;
const int ESP_BATSIG = 16;
int16_t adc0;

// See the following for generating UUIDs:
// https://www.uuidgenerator.net/

#define SERVICE_UUID           "4fafc201-1fb5-459e-8fcc-c5c9c331914b" // UART service UUID
#define CHARACTERISTIC_UUID_TX "beb5483e-36e1-4688-b7f5-ea07361b26a8"


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
   pinMode(ESP_RT, INPUT);
   pinMode(ESP_LT, INPUT);
   pinMode(ESP_FOG, INPUT);
   pinMode(ESP_SL, INPUT);
   pinMode(ESP_PL, INPUT);
   pinMode(ESP_PR, INPUT);
   pinMode(ESP_REV, INPUT);
   pinMode(ESP_IGNSIG, INPUT);
   pinMode(ESP_BATSIG, INPUT);

  // Create the BLE Device
  BLEDevice::init("UART Service");

  // Create the BLE Server
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pTxCharacteristic = pService->createCharacteristic(
                    CHARACTERISTIC_UUID_TX,
                    BLECharacteristic::PROPERTY_NOTIFY
                  );
                      
  pTxCharacteristic->addDescriptor(new BLE2902());


  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
  ads.begin();
}

void loop() {

    if (deviceConnected) {
      if (flag){
      Serial.println("connected");
      delay(1000); 
      //value = 0b00000000000000000000000000000000;
      //pTxCharacteristic->setValue(value);
      //pTxCharacteristic->notify();
      flag = false;
    }

      if( digitalRead(ESP_RT) == 0){
        value |= 0x02000000;
      delay(50); 
      }
      
      if(digitalRead(ESP_SL) == 0){
        value |= 0x20000000;
      delay(50);
      } 
      
      if(digitalRead(ESP_LT) == 0){
        value |= 0x01000000;
      delay(50); 
      }
      if(digitalRead(ESP_FOG) == 0){
        value |= 0x10000000;
      delay(50); 
      }
       
      if(digitalRead(ESP_PL) == 0){
        value |= 0x04000000;
      delay(50);
      } 
      
      if(digitalRead(ESP_PR) == 0){
        value |= 0x08000000;
      delay(50);
      } 
      
      if(digitalRead(ESP_REV) == 0){
        value |= 0x40000000;
      delay(50);
      } 

      if(digitalRead(ESP_IGNSIG) == 0){
        value |= 0x00010000;
      delay(50);
      } 
      
     if(digitalRead(ESP_BATSIG) == 0){
        value |= 0x80000000;
        delay(50);
      } 
        
adc0 = ads.readADC_SingleEnded(0);
Serial.print("breakSig = ");
Serial.println(adc0);
 input = adc0 / (65535 / 3.3);
 input = input / 0.27;
 input = input * 3.64;
 uint8_t tx = round(input);
        if (tx == 1){ 
        result =  0x00000100;
       }
       else if (tx == 2){
        result = 0x00001000;
       }
       else if (tx == 3){
        result = 0x00001100;
       }
       else if (tx == 4){
        result = 0x00000200;
       }
       else if (tx == 5){
        result = 0x00002000;
       }
       else if (tx == 6){
        result = 0x00000400;
       }
       else if (tx == 7){
        result = 0x00004000;
       }
       else if (tx == 8){
        result = 0x00000800;
       }
       else if (tx == 9){
        result = 0x00008000;
       }
       else if (tx == 10){
        result = 0x00000001;
       }
       else if (tx == 11){
        result = 0x00000010;
       }
       else if (tx == 12){
        result = 0x00000011;
       }
        
        result |= value;
        pTxCharacteristic->setValue(result);
        pTxCharacteristic->notify();
        Serial.print("Input: ");
        Serial.print(input);
        Serial.print("\tTX: ");
        Serial.println(tx);
        result = 0;
        value = 0;
        delay(100); // bluetooth stack will go into congestion, if too many packets are sent 
   }
    
   
    // disconnecting
    if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        flag =true;
        oldDeviceConnected = deviceConnected;
       
    }
    // connecting
    if (deviceConnected && !oldDeviceConnected) {
    // do stuff here on connecting
      oldDeviceConnected = deviceConnected;
       
    }
}
