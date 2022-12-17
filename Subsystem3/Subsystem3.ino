// Description: Gets and reads enviroment temperature through a sensor and
// submits to rgb led light.
// 
//References:
// https://create.arduino.cc/projecthub/muhammad-aqib/arduino-rgb-led-tutorial-fc003e
// https://create.arduino.cc/projecthub/techno_z/dht11-temperature-humidity-sensor-98b03b

#include <dht.h>
#define dht_apin A0 // Analog Pin sensor is connected to

dht DHT;
#include <Wire.h>
#define I2C_SLAVE_ADDRESS 12

 //int buttonPin = 12;
int red_light_pin= 11;
int green_light_pin = 10;
int blue_light_pin = 9;
//int buttonState = 0;
int n = 1;
void setup() {
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  delay(1000);//Wait before accessing Sensor
  Wire.begin(I2C_SLAVE_ADDRESS);
  // set pins on rgb led as output
  pinMode(red_light_pin, OUTPUT);
  pinMode(green_light_pin, OUTPUT);
  pinMode(blue_light_pin, OUTPUT);

  // initialize the pushbutton pin as an input:
 // pinMode(buttonPin, INPUT);
}

void requestEvents() {
  Serial.println(F("---> recieved request " ));
  Serial.print(F("Subsystem 2 sending value : "));
  Serial.println(n);
  Wire.write(n);
}

void receiveEvents(int numBytes) {
  Serial.println(F("----> received events"));
  n = Wire.read();
  Serial.print(numBytes);
  Serial.print(F("received value : "));
  Serial.println(n);
}

void loop() {
  DHT.read11(dht_apin); // read from module

  Serial.print("Current humidity = ");
  Serial.print(DHT.humidity);
  Serial.print("%  ");
  Serial.print("temperature = ");
  Serial.print(DHT.temperature); 
  Serial.println("C  ");

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH
  // light LED based on temp
  if (DHT.temperature > 28.00) { // this is too hot change if needed to something greater the the next else if
    // turn LED on:
    RGB_color(255, 0, 0); // Red
    Wire.onRequest(requestEvents);
  } else if ( DHT.temperature > 22.00) { // if you want to lower first if statement then lower this aswell
    // turn LED on:
    Wire.onRequest(requestEvents);
    RGB_color(255, 255, 255); // White
 } else { 
      RGB_color(0, 0, 255); // Blue
  }
  
  delay(5000);//Wait 5 seconds before accessing sensor again.
}

void RGB_color(int red_light_value, int green_light_value, int blue_light_value)
 {
  analogWrite(red_light_pin, red_light_value);
  analogWrite(green_light_pin, green_light_value);
  analogWrite(blue_light_pin, blue_light_value);
 }
