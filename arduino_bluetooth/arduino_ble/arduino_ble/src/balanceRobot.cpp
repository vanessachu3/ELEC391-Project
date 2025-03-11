#include <balanceRobot.h>
#include <PWM.h>
#include <pid.h>
float ax, ay, az, gx, gy, gz;                           // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0;                                      // initialize gyroscope integration constant
float gyrSampleRate;                                    // gyroscope sample rate

float k = 0.95;                                          // filter coefficient

#define MAXPWM 255

float getAngleSetup()
{
    Serial.begin(BAUD);
    if(!IMU.begin())                                    // IMU initialization
      {
          Serial.println("Failed to initialize IMU!");
          while(1);
      }
  
      gyrSampleRate = IMU.gyroscopeSampleRate();          // acquire sample rate for gyroscope angle
      Serial.println("Gyro sample rate");
      Serial.println(gyrSampleRate);
      return gyrSampleRate;
}
float getAngle(float gyrSampleRate)
{
    if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
    {
      IMU.readAcceleration(ax, ay, az);
      IMU.readGyroscope(gx, gy, gz);
  
      // calculate accelerometer angle
      accAngle = atan2(ay, az)*180/PI;
  
      // calculate gyroscope angle
      gyrAngle = gyrPrev - gx/gyrSampleRate;
      if(accAngle < 0.25 && accAngle > -0.25)
      {
        gyrAngle = 0;
      }
      gyrPrev = gyrAngle;
  
      // calculate filtered angle
      currAngle = k*(prevAngle - gx/gyrSampleRate)+(1-k)*accAngle;
      prevAngle = currAngle; 
    }
    return currAngle; 
}
void PWMfwrd(float scaleFactor) {
  
  moveRobot("FORWARD", scaleFactor);
  
}
  
void PWMbkwrd(float scaleFactor) {

  moveRobot("BACKWARDS", scaleFactor);
  
}

void balance(PID_t *pid, float currAngle)
{
  updatePID(pid, currAngle);
  float pidOut = getOutputPID(pid);

    
   
    if (pidOut < 0)
    {
      PWMbkwrd(-pidOut);
    }
    else{
      PWMfwrd(pidOut);
    }
    Serial.print(pidOut);
    Serial.print(" ");
    Serial.println(currAngle);
    //Serial.println(currAngle);
    
}
