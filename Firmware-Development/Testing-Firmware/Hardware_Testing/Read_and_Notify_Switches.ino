void read_notify_switches(){

/**********Switch-1 FUNCTION BLOCK*********/
  while (!digitalRead(sw_1)) {
    delay(100);
    press_time1 += 100;
  }

  if (press_time1 >= 100 && f1 == false) {
    value= 0b00000001;
    f1 = true;
    press_time1 = 0;
  }
    
  if (press_time1 >= 100 && f1 == true) {
    value= 0b00000010;
    f1 = false;
    press_time1 = 0;
  }

/**********Switch-2 FUNCTION BLOCK*********/
  while (!digitalRead(sw_2)) {
    delay(100);
    press_time2 += 100;
  }
  
  if (press_time2 >= 100 && f2 == false) {
    value= 0b00000011;
    f2 = true;
    press_time2 = 0;
  }    
  if (press_time2 >= 100 && f2 == true) {
    value= 0b00000100;
    f2 = false;
    press_time2 = 0;
  }

/**********Switch-3 FUNCTION BLOCK*********/
  while (!digitalRead(sw_3)) {
    delay(100);
    press_time3 += 100;
  }
  
  if (press_time3 >= 100 && f3 == false) {
    value= 0b00000101;
    f3 = true;
    press_time3 = 0;
  }
  if (press_time3 >= 100 && f3 == true) {
    value= 0b00000110;
    f3 = false;
    press_time3 = 0;
  }
  
/**********Switch-4 FUNCTION BLOCK*********/
  while (!digitalRead(sw_4)) {
    delay(100);
    press_time4 += 100;
  }

  if (press_time4 >= 100 && f4 == false) {
    value= 0b00000111;
    f4 = true;
    press_time4 = 0;
  }       
  if (press_time4 >= 100 && f4 == true) {
    value= 0b00001000;
    f4 = false;
    press_time4 = 0;
  }

/**********Switch-5 FUNCTION BLOCK*********/
  while (!digitalRead(sw_5)) {
    delay(100);
    press_time5 += 100;
  }
  
  if (press_time5 >= 100 && f5 == false) {
    value= 0b00001001;
    f5 = true;
    press_time5 = 0;
  } 
  if (press_time5 >= 100 && f5 == true) {
    value= 0b00001010;
    f5 = false;
    press_time5 = 0;
  }

/**********Switch-6 FUNCTION BLOCK*********/
  while (!digitalRead(sw_6)) {
    delay(100);
    press_time6 += 100;
  }
  
  if (press_time6 >= 100  && f6 == false) {
    value= 0b00001011;
    f6 = true;
    press_time6 = 0;
  }
  if (press_time6 >= 100  && f6 == true) {
    value= 0b00001100;
    f6 = false;
    press_time6 = 0;
  }

  //Serial.print("Value : ");
  //Serial.println(value);
    
  pcharacteristic_button->setValue(&value, 1);
  pcharacteristic_button->notify();

}
