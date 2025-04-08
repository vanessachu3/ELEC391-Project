#include <balanceRobot.h>

#include <PWM.h>

#include <pid.h>

float ax, ay, az, gx, gy, gz; // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0; // initialize gyroscope integration constant
float gyrSampleRate; // gyroscope sample rate

#define MAX_SPEED 0.5
float setPointDriver = 0.0;

const float k = 0.96; //0.98;                                          // filter coefficient

#define MAXPWM 255
float getAngleSetup() {
  Serial.begin(BAUD);
  if (!IMU.begin()) // IMU initialization
  {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }

  gyrSampleRate = IMU.gyroscopeSampleRate(); // acquire sample rate for gyroscope angle
  Serial.println("Gyro sample rate");
  Serial.println(gyrSampleRate);
  delay(100);
  IMU.readAcceleration(ax, ay, az);
  gyrPrev = atan(ay / az) * 180.0 / PI;
  return gyrSampleRate;
}
float getAngle(PID_t * pid, float gyrSampleRate) {
  if (IMU.accelerationAvailable() && IMU.gyroscopeAvailable()) {
    IMU.readAcceleration(ax, ay, az);
    IMU.readGyroscope(gx, gy, gz);

    // calculate accelerometer angle
    accAngle = atan(ay / az) * 180.0 / PI;

    // calculate gyroscope angle
    gyrAngle = gyrPrev - gx * gyrSampleRate;

    //gyrPrev = gyrAngle;

    // calculate filtered angle
    if (abs(abs(currAngle)+abs(pid->desiredAngle)) < 0.25) {
      gyrAngle = accAngle;// pid -> desiredAngle;
    }

    currAngle = k * (gyrAngle) + (1 - k) * accAngle;
    gyrPrev = currAngle;

    return currAngle;
  }
  return 1000;

}
void PWMfwrd(float scaleFactor, bool dlf, bool drt) {

  moveRobot("FORWARD", scaleFactor, dlf, drt);

}

void PWMbkwrd(float scaleFactor, bool dlf, bool drt) {

  moveRobot("BACKWARDS", scaleFactor, dlf, drt);

}

void balance(PID_t * pid, float currAngle, float sampleSec, bool dfw, bool dbw, bool dlf, bool drt) {
  if(dfw)
  {
    if(abs(pid->desiredAngle) < 1.5) //FWRD IS -VE 
      pid->desiredAngle -= 0.3;
  }
  else if(dbw)
  {
    if(abs(pid->desiredAngle) < 1.2) //BKWRD IS +VE
    pid->desiredAngle += 0.01;
  }
  else
  {
    pid->desiredAngle = DESIRED_ANGLE;
    //setPointDriver = 0;
  }

  
  updatePID(pid, currAngle, sampleSec);
  float pidOut = getOutputPID(pid);
  if (pidOut < 0) {
    PWMbkwrd(-pidOut, dlf, drt);
    //PWMbkwrd(0);
  } else {
    //PWMfwrd(0);
    PWMfwrd(pidOut, dlf, drt);
  }

  //Serial.print(pid->e1);
  //Serial.print(" ");
  //Serial.print(pid->e0);
  //Serial.print(" ");
  //Serial.print(pid->iTerm);
  //Serial.print(" ");
  //Serial.println(currAngle);
  //Serial.println(pid->u0);

}