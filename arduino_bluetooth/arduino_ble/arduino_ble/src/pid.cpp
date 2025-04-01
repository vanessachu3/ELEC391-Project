#include <pid.h>
#include <math.h>
#include <Arduino.h>
#include <stdio.h>
bool serialCommandReady = false;
String inputString = "";
PID_t pid;
#define MAX_PIDOUT 255.0
#define MIN_PIDOUT -255.0
float  kpIncrement = 1;
float  kiIncrement = 1;
float  kdIncrement = 0.01;
#define NUMDIGITS 9

void processSerialInput(PID_t *pid) {
    if (Serial.available() > 0) {
      Serial.println("*");
  
      char inChar = (char)Serial.read();
      
      // Serial.print("Received char code: ");
      // Serial.println((int)inChar);
  
      // if (inChar == '\n' || inChar == '\r') {
      serialCommandReady = true;
      // } else {
        // Add character to input string
      inputString += inChar;
      Serial.println(inputString);
      // }
    }
    
    if (serialCommandReady) {
      Serial.println("Command Recieved");
      inputString.trim();  // Remove whitespace
      
      // Process commands
      if (inputString.equals("q") || inputString.equals("Q")) {
        pid->kp += kpIncrement;
        Serial.print("Kp increased to: ");
        Serial.println(pid->kp, NUMDIGITS);
      }
      else if (inputString.equals("a") || inputString.equals("A")) {
        pid->kp -= kpIncrement;
        Serial.print("Kp decreased to: ");
        if (pid->kp < 0) pid->kp = 0;
        Serial.println(pid->kp, NUMDIGITS);
      }
      else if (inputString.equals("e") || inputString.equals("E")) {
        pid->ki += kiIncrement;
        
        Serial.print("Ki increased to: ");
        Serial.println(pid->ki, NUMDIGITS);
      }
      else if (inputString.equals("d") || inputString.equals("D")) {
        pid->ki -= kiIncrement;
        if (pid->ki < 0) pid->ki = 0;
        Serial.print("Ki decreased to: ");
        Serial.println(pid->ki , NUMDIGITS);
      }
      else if (inputString.equals("r") || inputString.equals("R")) {
        pid->kd += kdIncrement;
        
        Serial.print("Kd increased to: ");
        Serial.println(pid->kd, NUMDIGITS);
      }
      else if (inputString.equals("f") || inputString.equals("F")) {
        pid->kd -= kdIncrement;
        if (pid->kd < 0) pid->kd = 0;
        Serial.print("Kd decreased to: ");
        Serial.println(pid->kd, NUMDIGITS);
      }
      else if (inputString.equals("p") || inputString.equals("P")) {
        kpIncrement *= 10;
        Serial.print("Kp increment increased to: ");
        Serial.println(kpIncrement, NUMDIGITS);
      }
      else if (inputString.equals("l") || inputString.equals("L")) {
        kpIncrement /= 10;
        Serial.print("Kp increment decreased to: ");
        Serial.println(kpIncrement, NUMDIGITS);
      }
      else if (inputString.equals("o") || inputString.equals("O")) {
        kiIncrement *= 10;
        Serial.print("Ki increment increased to: ");
        Serial.println(kiIncrement, NUMDIGITS);
      }
      else if (inputString.equals("k") || inputString.equals("K")) {
        kiIncrement /= 10;
        Serial.print("Ki increment decreased to: ");
        Serial.println(kiIncrement, NUMDIGITS);
      }
      else if (inputString.equals("i") || inputString.equals("I")) {
        kdIncrement *= 10;
        Serial.print("Kd increment increased to: ");
        Serial.println(kdIncrement, NUMDIGITS);
      }
      else if (inputString.equals("j") || inputString.equals("J")) {
        kdIncrement /= 10;
        Serial.print("Kd increment decreased to: ");
        Serial.println(kdIncrement, NUMDIGITS);
      }
      else if (inputString.equals("t") || inputString.equals("T")) {
        pid->desiredAngle += 0.1;
        Serial.print("Desired angle increased to: ");
        Serial.println(pid->desiredAngle, NUMDIGITS);
      }
        else if (inputString.equals("g") || inputString.equals("G")) {
            pid->desiredAngle -= 0.1;
            Serial.print("Desired angle decreased to: ");
            Serial.println(pid->desiredAngle, NUMDIGITS);
        }
      

      // else if (inputString.startsWith("s") || inputString.startsWith("S")) {
      //   // Set Kp to specific value: format "p=25.5"
      //   float newKp = inputString.substring(2).toFloat();
      //   if (newKp != 0.0 || inputString.substring(2).equals("0")) {
      //     Kp = newKp;
      //     Serial.print("Kp set to: ");
      //     Serial.println(Kp, 6);
      //   }
      // }
      else if (inputString.equals("s")) {
        // Print current PID values
        Serial.print(", Output=");
        Serial.print(pid->u0, NUMDIGITS);
        Serial.print(", Angle=");
        Serial.print(pid->angleRead, NUMDIGITS);
        Serial.print(", Desired Angle=");
        Serial.println(pid->desiredAngle, NUMDIGITS);
        Serial.print("Kp=");
        Serial.print(pid->kp, NUMDIGITS);
        Serial.print(", Ki=");
        Serial.print(pid->ki, NUMDIGITS);
        Serial.print(", Kd=");
        Serial.println(pid->kd, NUMDIGITS);
      }
      else if (inputString.length() > 0) {
        Serial.println("Commands: p+ (increase), p- (decrease), p=25.5 (set), pid (show values)");
      }
      
      // Reset for next command
      inputString = "";
      serialCommandReady = false;
    }
  }
void updatePID(PID_t *pid, double angle, float sampleSec) {
    // Update previous error and output values
    pid->e1 = pid->e0; //e[k-1]
    pid->Ts = sampleSec; //Ts
    if (pid->Ts <=0)
    {
        pid->Ts = 1.0/99.84;
    }
    // Calculate current error
    pid->angleRead = angle; 
    pid->e0 = pid->desiredAngle - pid->angleRead; //e[k]

    // Proportional term
    pid->pTerm = pid->kp * pid->e0; // kp * e[k]

  
    // Integral term with anti-windup

      pid->iTemp += (pid->e0) * pid->Ts;
  

    float maxIntegral = 15.0;
    if (pid->iTemp > maxIntegral) pid->iTemp = maxIntegral;
    else if (pid->iTemp < -maxIntegral) pid->iTemp = -maxIntegral;
    
    pid->iTerm = pid->ki * pid->iTemp;

    // Derivative term
    // Compute the raw derivative
    float dRaw = 0.0;
    
    dRaw = (pid->e0 - pid->e1) / pid->Ts;

    // Apply a low-pass filter using the filter coefficient
    pid->dTemp = dRaw;

    pid->dTerm = pid->kd * pid->dTemp;

    // Compute final output
    pid->u0 = pid->pTerm + pid->iTerm + pid->dTerm;

    // Safety shut-off for large angles
    if (angle > 37.0 || angle < -37.0) {
        pid->u0 = 0.0;
    }
    //else if(angle < pid->desiredAngle + 0.5 && angle > pid->desiredAngle - 0.5)
    //{
    //
    //    pid->u0 = 0.0;
    //}

    // Output saturation
    if (pid->u0 > MAX_PIDOUT) pid->u0 = MAX_PIDOUT;
    else if (pid->u0 < MIN_PIDOUT) pid->u0 = MIN_PIDOUT;

    //else if (pid->e0 < 0.05 && pid->e0 > -0.05)
    //{
    //    pid->u0 = 0.0;
    //}
    //Serial.println(pid->angleRead);
}

void updateDesiredAngle(PID_t *pid, float angle) {
    pid->desiredAngle = angle;
}

void updateDesiredAngleCommmand(PID_t *pid, const char* command) {
    // Parse the command to extract the desired angle
    if (strcmp(command, "INCREASE") == 0) {
        pid->desiredAngle += 0.2;
    }
    else if((strcmp(command, "DECREASE") == 0)) {
        pid->desiredAngle -= 0.2;
    }
    else if(strcmp(command,"DEFAULT") == 0){
        pid->desiredAngle = DESIRED_ANGLE;
    }
}


float getOutputPID(PID_t *pid)
{
    
    return pid->u0;
}

