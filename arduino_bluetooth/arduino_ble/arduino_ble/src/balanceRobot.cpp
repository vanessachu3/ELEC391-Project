#include <balanceRobot.h>
#include <PWM.h>
#include <pid.h>
float ax, ay, az, gx, gy, gz;                           // IMU acceleration | IMU gyroscope
float accAngle, gyrAngle = 0, currAngle, prevAngle = 0; // initialize accAngle and gyrAngle
float gyrPrev = 0;                                      // initialize gyroscope integration constant
float gyrSampleRate;                                    // gyroscope sample rate

const float k = 0.96;//0.98;                                          // filter coefficient

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
      delay(100);
      IMU.readAcceleration(ax, ay, az);
      gyrPrev = atan(ay/az )*180.0/PI;
      return gyrSampleRate;
}
float getAngle(PID_t *pid, float gyrSampleRate)
{
    if(IMU.accelerationAvailable() && IMU.gyroscopeAvailable())
    {
      IMU.readAcceleration(ax, ay, az);
      IMU.readGyroscope(gx, gy, gz);

      // calculate accelerometer angle
      accAngle = atan(ay/az )*180.0/PI;//+0.2;
      //Serial.println(accAngle);
      
      // calculate gyroscope angle
      gyrAngle = gyrPrev - gx*gyrSampleRate;
      
      
      gyrPrev = gyrAngle;
      //Serial.println(gyrAngle);
      
      // calculate filtered angle
      if(abs(abs(accAngle)+abs(pid->desiredAngle))<0.25)
      {
       gyrAngle = pid->desiredAngle;
      }
      
      currAngle = k*(gyrAngle)+(1-k)*accAngle;
      
      //Serial.println(currAngle);


      //Serial.println(currAngle);
      //prevAngle = currAngle; 
    
    //else{
    //  Serial.println("IMU not available");
    //}
    return currAngle; 
    }
    return 0;
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
      //PWMbkwrd(0);
    }
    else{
      //PWMfwrd(0);
      PWMfwrd(pidOut);
    }
    //Serial.print(pid->e1);
    //Serial.print(" ");
    //Serial.print(pid->e0);
    //Serial.print(" ");
    //Serial.print(pid->iTerm);
    //Serial.print(" ");
    //Serial.println(currAngle);
    
}
