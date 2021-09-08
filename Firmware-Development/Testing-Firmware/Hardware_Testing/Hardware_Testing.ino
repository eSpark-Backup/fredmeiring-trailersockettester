#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>
#include <BLE2902.h>
#include <Wire.h>
#include <MPU9250_asukiaaa.h>

#ifdef _ESP32_HAL_I2C_H_
#define SDA_PIN 21
#define SCL_PIN 22
#endif


#define SERVICE_UUID                  BLEUUID((uint16_t)0xAAAA)
#define BUTTON_CHARACTERISTIC_UUID    BLEUUID((uint16_t)0xA0A0)
#define LED_CHARACTERISTIC_UUID       BLEUUID((uint16_t)0xA0A1)
#define ACCELERO_CHARACTERISTICS_UUID BLEUUID((uint16_t)0xA0A2)
#define GYRO_CHARACTERISTICS_UUID     BLEUUID((uint16_t)0xA0A3)


//#define SERVICE_UUID_1                "0000AAAA-0000-1000-8000-00805f9b34fb" Service UUID for the ESP-32(We have running only 1 Service)
//#define BUTTON_CHARACTERISTIC_UUID    "0000A0A0-0000-1000-8000-00805f9b34fb" Button Characteristic UUID for the notification of Harware Button 
//#define LED_CHARACTERISTIC_UUID       "0000A0A1-0000-1000-8000-00805f9b34fb" Led Characteristic UUID for displaying data from Smartphone to Hardware

//#define ACCELERO_CHARACTERISTICS_UUID "0000A0A2-0000-1000-8000-00805f9b34fb"
//#define GYRO_CHARACTERISTICS_UUID     "0000A0A3-0000-1000-8000-00805f9b34fb"



#define sw_1       19
#define sw_2       18
#define sw_3       5
#define sw_4       4
#define sw_5       16
#define sw_6       17

#define led_1      33
#define led_2      25
#define led_3      26
#define led_4      27
#define led_5      14
#define led_6      12

#define vib_motor  13

byte led_pins[] ={led_1,led_2,led_3,led_4,led_5,led_6};


bool deviceConnected = false;
bool oldDeviceConnected = false;
uint8_t value = 0b00000000;
bool f1, f2, f3, f4, f5, f6 = false;
uint16_t press_time1, press_time2, press_time3, press_time4, press_time5, press_time6 = 0;

String val,val_1;
uint8_t led_value = 0;


MPU9250 mySensor;
uint8_t sensorId;
float aX, aY, aZ, aSqrt, gX, gY, gZ, mDirection, mX, mY, mZ;
char val_acc[20],val_gyro[30];
String A,AX,AY,AZ;
String G,GX,GY,GZ;

String C=",";



BLEServer* pServer = NULL;

BLECharacteristic *pcharacteristic_button = NULL;
BLECharacteristic *pcharacteristic_led  = NULL;
BLECharacteristic* pcharacteristic_ACC= NULL;
BLECharacteristic* pcharacteristic_GYRO= NULL;

class LedCharacteristicCallback: public BLECharacteristicCallbacks {
    void onWrite(BLECharacteristic *pcharacteristic_led) {
      std::string value = pcharacteristic_led->getValue();
      if(value.length()>0){for (int i = 0; i < value.length(); i++){val+=value[i];}}
      
      Serial.println("========================================================");
      Serial.print("New Value: ");
           if(val=="on"){digitalWrite(vib_motor,HIGH);Serial.println (val);}
      else if(val=="off"){digitalWrite(vib_motor,LOW);Serial.println (val);}
      else{
      led_value=val.toInt();
      Serial.println(led_value);
      Serial.println(led_value,BIN);
      led_value=~led_value;
      
      Serial.println("========================================================");
      
      for(int i=0;i<6;i++){
        bool state= bitRead(led_value,i);
        digitalWrite(led_pins[i],state);
        
      }
      led_value=0;
      }
      val="";
       
    }
};

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
  
#ifdef _ESP32_HAL_I2C_H_ // For ESP32
  Wire.begin(SDA_PIN, SCL_PIN); // SDA, SCL
#else
  Wire.begin();
#endif

  mySensor.setWire(&Wire);
  mySensor.beginAccel();
  mySensor.beginGyro();
  sensorId = mySensor.readId();

  
  pinMode(sw_1,INPUT_PULLUP);
  pinMode(sw_2,INPUT_PULLUP);
  pinMode(sw_3,INPUT_PULLUP);
  pinMode(sw_4,INPUT_PULLUP);
  pinMode(sw_5,INPUT_PULLUP);
  pinMode(sw_6,INPUT_PULLUP);
  
  pinMode(led_1,OUTPUT);
  pinMode(led_2,OUTPUT);
  pinMode(led_3,OUTPUT);
  pinMode(led_4,OUTPUT);
  pinMode(led_5,OUTPUT);
  pinMode(led_6,OUTPUT);

  pinMode(vib_motor,OUTPUT);

  digitalWrite(led_1,HIGH);
  digitalWrite(led_2,HIGH);
  digitalWrite(led_3,HIGH);
  digitalWrite(led_4,HIGH);
  digitalWrite(led_5,HIGH);
  digitalWrite(led_6,HIGH);

  digitalWrite(vib_motor,LOW);

  
  BLEDevice::init("Hardware Testing Board");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

    BLEService *pService = pServer->createService(SERVICE_UUID);

  //////////////////////////BUTTON_CHARACTERISTIC//////////////////////////////////////////////////////////
  pcharacteristic_button = new BLECharacteristic(BUTTON_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pService->addCharacteristic(pcharacteristic_button);
  pcharacteristic_button->addDescriptor(new BLE2902());                                                                 
////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////LED_CHARACTERISTIC////////////////////////////////////////////////////////////
  pcharacteristic_led = new BLECharacteristic(LED_CHARACTERISTIC_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY | BLECharacteristic::PROPERTY_WRITE);
  pcharacteristic_led->setCallbacks(new LedCharacteristicCallback());
  pService->addCharacteristic(pcharacteristic_led);    
  pcharacteristic_led->addDescriptor(new BLE2902());                                                                     
////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////ACCELEROMETER_CHARACTERISTIC//////////////////////////////////////////////////////////
  pcharacteristic_ACC = new BLECharacteristic(ACCELERO_CHARACTERISTICS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pService->addCharacteristic(pcharacteristic_ACC);    
  pcharacteristic_ACC->addDescriptor(new BLE2902());                                                                      
////////////////////////////////////////////////////////////////////////////////////////////////////////

//////////////////////////GYRO_CHARACTERISTIC//////////////////////////////////////////////////////////
  pcharacteristic_GYRO = new BLECharacteristic(GYRO_CHARACTERISTICS_UUID, BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);
  pService->addCharacteristic(pcharacteristic_GYRO);    
  pcharacteristic_GYRO->addDescriptor(new BLE2902());                                                                      
////////////////////////////////////////////////////////////////////////////////////////////////////////


  pService->start();
  pServer->getAdvertising()->start();
 // BLEAdvertising *pAdvertising = pServer->getAdvertising();
 // pAdvertising->setAppearance((uint16_t)(786)); //Generic Display
 // pAdvertising->start();

}

void loop() {
  
  if (deviceConnected) {

        read_notify_switches();
        //read_notify_accelero();
        //read_notify_gyro();

        delay(500);
    }
  if (!deviceConnected && oldDeviceConnected) {
        delay(500); // give the bluetooth stack the chance to get things ready
        pServer->startAdvertising(); // restart advertising
        Serial.println("start advertising");
        oldDeviceConnected = deviceConnected;
   }

  if (deviceConnected && !oldDeviceConnected) {
        // do stuff here on connecting
        oldDeviceConnected = deviceConnected;
   }
}
