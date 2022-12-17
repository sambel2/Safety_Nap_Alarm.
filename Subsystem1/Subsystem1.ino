/*
Arduino: Get user input for time and display on lcd.

// 1- Sergio Ambelis Diaz - UIN: 659890446
// 2- Project - Nap Alarm
// 3- Description - what is this code supposed to do? Sets an alarm clock countdown timer with the use of 3 push buttons. Button 1
      increments timer by 1 minute. Button 2 decrements timer by 1 minute. Button 3 starts/stops the timer or resets timer.
// 4- References - https://create.arduino.cc/projecthub/tittiamo68/alarm-clock-f61bad alarm clock countdown timer.
      https://docs.arduino.cc/learn/electronics/lcd-displays for LCD screen.
*/ 

// include the library code:
#include <LiquidCrystal.h>
#include <TimeLib.h>
#include <Wire.h>
#define I2C_SLAVE_ADDRESS 11


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 13;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
const int  up_buttonPin   = 10;    // the pin that the pushbutton is attached to
const int  down_buttonPin = 9;
const int  startButton = 2;

// Variables will change:

int buttonPushCounter = 0;   // counter for the number of button presses
int up_buttonState = 0;         // current state of the up button
int up_lastButtonState = 0;     // previous state of the up button
int startState = 0;           //  current state of start button
int down_buttonState = 0;         // current state of the up button
int down_lastButtonState = 0;     // previous state of the up button
bool startB = false;
unsigned long minutes = 0, seconds = 0;
int n = 0;
int x = 1;
bool check = true;

void startTime() {
  startB = true;
//  had startB = !startB but button was very sensative
  delay(400);
}

void inputTime() {
  lcd.setCursor(2,0);  
  lcd.print("Enter Minutes:");
  lcd.setCursor(2,1);
  lcd.print(buttonPushCounter);
  delay(100);
}

void setup() {
  lcd.begin(16, 2);  // 16 columns and 2 rows
  Wire.begin(I2C_SLAVE_ADDRESS);
  Serial.begin(9600);
  pinMode( up_buttonPin, INPUT);
  pinMode( down_buttonPin, INPUT);
  pinMode (startButton, INPUT); 
 
  // interrupt function for start/stop/reset button
  attachInterrupt(digitalPinToInterrupt(startButton), startTime, RISING);
}


void requestEvents() {
  Serial.println(F("---> recieved request " ));
  Serial.print(F("Subsystem 1 sending value : "));
  Serial.println(x);
  Wire.write(x);
  // bug were subsystem 1 continously Tx's
  x = 0;
  Serial.println("after we write");
  
}

void loop() {
  minutes = buttonPushCounter;
  lcd.clear();
  while (startB == true) {
      check = true;
      lcd.setCursor(0, 0);
      lcd.print("Alarm timer");
      lcd.setCursor(0, 1);
      // sprintf didn't print updated seconds 
      // I used old school lcd.print instead
      lcd.print(minutes);
      lcd.print(" mins ");
      lcd.print(seconds);
      lcd.print(" secs");   
      
//  ****   For Debugging purposes:   ****
      Serial.print("1. the minute is ");
      Serial.println(minutes);
      Serial.print("1. the seconds is: ");
      Serial.println(seconds);

      // end of timer
        if (minutes == 0 && seconds == 0) {
          Serial.println("Stop the clock!");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Wake Up");
          delay(2000);
          Serial.println("Packet sent");
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Sending alarm...");
         // delay(1000);
          if(check) {
            x = 1;
            Wire.onRequest(requestEvents);
            Serial.println("inside bool");
            check = false;
          }
          Serial.println("outside the write event");
          //delay(100);
          startB = false;
          break;
        }      
       // if we our seconds is at 0 and minutes is not 0 (edge case)
      if (seconds == 0 && minutes != 0 ) {
        minutes--;
        seconds = 60; // change to 60 
      }
      delay(1000);
      seconds --; // decrement seconds

      // **** For Debugging purposes! ***
      Serial.print("2. the minute is ");
      Serial.println(minutes);
      Serial.print("2. the seconds is: ");
      Serial.println(seconds);      
  }
  seconds = 0;
  minutes = 0;
  checkUp();
  checkDown();
  inputTime();
}

// for the up button - increment
void checkUp(){
  up_buttonState = digitalRead(up_buttonPin);
  // compare the buttonState to its previous state
  if (up_buttonState != up_lastButtonState) {
    // if the state has changed, increment the counter
    if (up_buttonState == LOW) {
        buttonPushCounter++;
    }
    // Delay a little bit to avoid bouncing
    delay(100);
  }
  // save the current state as the last state, for next time through the loop
  up_lastButtonState = up_buttonState;
}

// for the down button - decrement
void checkDown(){
  down_buttonState = digitalRead(down_buttonPin);
  // compare the buttonState to its previous state
  if (down_buttonState != down_lastButtonState) {
    // if the state has changed, increment the counter
    if (down_buttonState == LOW) {
      if (buttonPushCounter > 0)
      buttonPushCounter--;
    }
    // Delay a little bit to avoid bouncing
    delay(100);
  }
  // save the current state as the last state, for next time through the loop
  down_lastButtonState = down_buttonState;
}
