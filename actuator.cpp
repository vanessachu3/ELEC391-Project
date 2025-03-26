#include <Arduino.h>
#include <Servo.h>

Servo myservo1;  // create servo object to control a servo
Servo myservo2;


// twelve servo objects can be created on most boards
int pos = 0;    // variable to store the servo position

void setup() {
  Serial.begin(115200);

  myservo1.attach(A7);  // attaches the servo on pin 9 to the servo object
  myservo2.attach(A6);

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
  for (int pos1 = 0, pos2 = 180; pos1 < 180 && pos2 > 0; pos1 += 1, pos2 -= 1) { // goes from 0 degrees to 180 degrees
    myservo1.write(pos1);                // tell servo to go to position in variable 'pos'
    myservo2.write(pos2);
    delay(15);                         // waits 15ms for the servo to reach the position
    Serial.println(pos1);
  }

  for (int pos1 = 180, pos2 = 0; pos1 > 0 && pos2 < 180; pos1 -= 1, pos2 += 1) { // goes from 180 degrees to 0 degrees
    myservo1.write(pos1);                // tell servo to go to position in variable 'pos'
    myservo2.write(pos2);
    delay(30);                         // waits 15ms for the servo to reach the position
  }
  //Serial.println(pos1 + "\t" + pos2);
}
