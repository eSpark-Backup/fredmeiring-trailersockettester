
bool a=0,b=0,c=0,d=0,e=0,f=0;


void setup() {
  Serial.begin(115200);
pinMode(19,INPUT_PULLUP);
pinMode(18,INPUT_PULLUP);
pinMode(5,INPUT_PULLUP);
pinMode(17,INPUT_PULLUP);
pinMode(16,INPUT_PULLUP);
pinMode(4,INPUT_PULLUP);

pinMode(33,OUTPUT);
pinMode(25,OUTPUT);
pinMode(26,OUTPUT);
pinMode(27,OUTPUT);
pinMode(14,OUTPUT);
pinMode(12,OUTPUT);

digitalWrite(33,HIGH);
digitalWrite(25,HIGH);
digitalWrite(26,HIGH);
digitalWrite(27,HIGH);
digitalWrite(14,HIGH);
digitalWrite(12,HIGH);

}

void loop() {     
if(digitalRead(19)){a=1;digitalWrite(33,HIGH);}else {a=0;digitalWrite(33,LOW);}
if(digitalRead(18)){b=1;digitalWrite(25,HIGH);}else {b=0;digitalWrite(25,LOW);}
if(digitalRead(5)){c=1;digitalWrite(26,HIGH);}else {c=0;digitalWrite(26,LOW);}
if(digitalRead(4)){d=1;digitalWrite(27,HIGH);}else {d=0;digitalWrite(27,LOW);}
if(digitalRead(16)){e=1;digitalWrite(14,HIGH);}else {e=0;digitalWrite(14,LOW);}
if(digitalRead(17)){f=1;digitalWrite(12,HIGH);}else {f=0;digitalWrite(12,LOW);}

Serial.print(a);
Serial.print(b);
Serial.print(c);
Serial.print(d);
Serial.print(e);
Serial.print(f);
Serial.println();

delay(500);
}
