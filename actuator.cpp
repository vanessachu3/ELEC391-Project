#include <Arduino.h>
#include <Servo.h>

Servo myservo;  // create servo object to control a servo

// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);

  myservo.attach(D9);  // attaches the servo on pin 9 to the servo object
}

void loop() {
  if(myservo.attached()){
    Serial.println("actuator attached");
  }else{
    Serial.println("actuator not attached");
  }
  
  for (pos = 0; pos < 180; pos += 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  Serial.println(pos);

  for (pos = 180; pos > 0; pos -= 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    //analogWrite(D9, map(pos, 0, 180, 0, 255));
    delay(30);                       // waits 15ms for the servo to reach the position
  }
  Serial.println(pos);
}
