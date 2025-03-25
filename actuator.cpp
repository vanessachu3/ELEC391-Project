#include <Arduino.h>
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;


// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);

  myservo1.attach(D9);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(D8);

  if(myservo1.attached()){
    Serial.println("actuator 1 attached");
  }else{
    Serial.println("actuator 1 not attached");
  }

  if(myservo2.attached()){
    Serial.println("actuator 2 attached");
  }else{
    Serial.println("actuator 2 not attached");
  }
}

void loop() {
  for (pos = 0; pos < 180; pos += 1) { // goes from 0 degrees to 180 degrees
    myservo1.write(pos);                // tell servo to go to position in variable 'pos'
    delay(30);                         // waits 15ms for the servo to reach the position
  }
  Serial.println(pos);

  for (pos = 180; pos > 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos);                // tell servo to go to position in variable 'pos'
    delay(30);                         // waits 15ms for the servo to reach the position
  }
  Serial.println(pos);
}
