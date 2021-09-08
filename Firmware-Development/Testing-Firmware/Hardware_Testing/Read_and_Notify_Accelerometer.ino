void read_notify_accelero(){
        mySensor.accelUpdate();
        aX = mySensor.accelX();
        aY = mySensor.accelY();
        aZ = mySensor.accelZ();
        
        AX=String(aX)+",";
        AY=String(aY)+",";
        AZ=String(aZ);
        
        A=AX+AY+AZ;
        A.toCharArray(val_acc,20);
        std::string myString((char*)val_acc); 
        
//        Serial.print("AX: " + String(aX) + " ");
//        Serial.print("  AY: " + String(aY) + " ");
//        Serial.println("  AZ: " + String(aZ));
          
        pcharacteristic_ACC->setValue(myString);
        pcharacteristic_ACC->notify();
}
