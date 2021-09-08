void read_notify_gyro(){
        mySensor.gyroUpdate();
        gX = mySensor.gyroX();
        gY = mySensor.gyroY();
        gZ = mySensor.gyroZ();

        GX=String(gX)+",";
        GY=String(gY)+",";
        GZ=String(gZ);
       
        G=GX+GY+GZ;
        G.toCharArray(val_gyro,30);
        std::string myString_1((char*)val_gyro);

        pcharacteristic_GYRO->setValue(myString_1);
        pcharacteristic_GYRO->notify();
}
