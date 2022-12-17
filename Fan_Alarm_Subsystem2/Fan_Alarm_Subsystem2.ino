// Safety Nap Alarm Subsystem 2
// written by Ivan Zhang

#include <SoftwareSerial.h>
#include <Wire.h>

const int buzzButtonPin = 2;
const int motorButtonPin = 8;
const int buzzPin = 4;
const int motorPin = 5;
# define I2C_SLAVE1_ADDRESS 11
# define I2C_SLAVE2_ADDRESS 12


int buzzButtonState = 0;
int motorButtonState = 0; 
int buzzpressed = 0;
bool motorpressed = false;
int alarm;
int temp;

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
  pinMode(buzzButtonPin, INPUT);
  pinMode(motorButtonPin, INPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(motorPin, OUTPUT);
 Serial.println("Start");
}

bool check = false;
bool check2 = false;
void loop() {
  motorButtonState = digitalRead(motorButtonPin);
   if (motorButtonState == HIGH) {
    Serial.println("Stop fan");
    motorpressed = true;
    delay(500);
    while(digitalRead(motorButtonPin) == HIGH) {}
 }
  
   buzzButtonState = digitalRead(buzzButtonPin);
   if (buzzButtonState == HIGH) {
    Serial.println("Stop alarm");
    buzzpressed = 1;
    delay(500);
    while(digitalRead(buzzButtonPin) == HIGH) {}
   }
  
    Wire.requestFrom(I2C_SLAVE1_ADDRESS, 1);
    alarm = Wire.read();
    Serial.print(F("received value from sub1: "));
    Serial.println(alarm);
    if(alarm == 1) {
        buzzpressed = 0;
    }
    
  
  if(buzzpressed == 0 && alarm == 1) { 
    tone(buzzPin, 1000); // turn on buzzer
    Serial.println("buzzing");
  }
  if(buzzpressed == 1) {
    noTone(buzzPin);
    alarm = 0;
    buzzpressed = 0;
  }

  if(!check2){
  Wire.requestFrom(I2C_SLAVE2_ADDRESS, 1);
  temp = Wire.read();
  Serial.print(F("received value from sub3: "));
  Serial.println(temp);
  if(temp == 1) {
        check2 = true;
        motorpressed = false;
      }
  }

  if(motorpressed == false && temp == 1){
    analogWrite(motorPin, 255); 
    Serial.println("fan on");
  
    // turn on fan when hot and fan wasnt on before
  }
  if(motorpressed == true) {
    analogWrite(motorPin, 0);
    temp = !temp;
    motorpressed = false;
    
  }

}
