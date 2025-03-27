#include <balanceRobot.h>
#include <PWM.h>
#include <pid.h>
float ax, ay, az, gx, gy, gz;                           // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0;                                      // initialize gyroscope integration constant
float gyrSampleRate;                                    // gyroscope sample rate

float k = 0.98;                                          // filter coefficient

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
      delay(1000);
      IMU.readAcceleration(ax, ay, az);
      gyrPrev = atan2(ay, az)*180/PI;
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
      //Serial.println(accAngle);
      
      // calculate gyroscope angle
      gyrAngle = gyrPrev - gx*gyrSampleRate;
      
      
      gyrPrev = gyrAngle;
      //Serial.println(gyrAngle);
      
      // calculate filtered angle
      if(abs(accAngle)<0.35)
      {
        gyrAngle = 0;
      }
      else{
      currAngle = k*(gyrAngle)+(1-k)*accAngle;
      }
      //Serial.println(currAngle);
      //prevAngle = currAngle; 
    }
    return currAngle; 
}
void PWMfwrd(float scaleFactor) {
  
  moveRobot("FORWARD", scaleFactor);
  
}
  
void PWMbkwrd(float scaleFactor) {

  moveRobot("BACKWARDS", scaleFactor);
  
}

void balance(PID_t *pid, float currAngle,float sampleSec)
{
  updatePID(pid, currAngle,sampleSec);
  float pidOut = getOutputPID(pid);
    if (pidOut < 0)
    {
      PWMbkwrd(-pidOut);
    }
    else{
      PWMfwrd(pidOut);
    }
    //Serial.print(pidOut);
    //Serial.print(" ");
    //Serial.println(currAngle);
    //Serial.println(currAngle);
    
}
