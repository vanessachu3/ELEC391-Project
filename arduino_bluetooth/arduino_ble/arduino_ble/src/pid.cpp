#include <pid.h>
#include <math.h>
#include <Arduino.h>
#include <stdio.h>
bool serialCommandReady = false;
String inputString = "";
PID_t pid;
#define MAX_PIDOUT 1.0
#define MIN_PIDOUT -1.0
float  kpIncrement = 0.000001;
float  kiIncrement = 0.000001;
float  kdIncrement = 0.000001;
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
        Serial.print(", Filter=");
        Serial.println(pid->filterCoeff, NUMDIGITS);
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

    // Calculate current error
    pid->angleRead = angle; 
    pid->e0 = pid->desiredAngle - pid->angleRead; //e[k]

    // Proportional term
    pid->pTerm = pid->kp * pid->e0; // kp * e[k]

    // Integral term with anti-windup
    pid->iTemp += (pid->e0+pid->e1) * pid->Ts/2; // e_i[k-1] + Ts/2 ( e[k-1] + e[k])
    const float maxIntegral = 1.0;
    if (pid->iTemp > maxIntegral) pid->iTemp = maxIntegral;
    else if (pid->iTemp < -maxIntegral) pid->iTemp = -maxIntegral;
    
    pid->iTerm = pid->ki * pid->iTemp;

    // Derivative term
    // Compute the raw derivative
    float dRaw = 0.0;
    if (pid->Ts <=0)
    {
        pid->Ts = 1/99.86;
    }
    dRaw = (pid->e0 - pid->e1) / pid->Ts;

    // Apply a low-pass filter using the filter coefficient
    pid->dTemp = pid->filterCoeff * dRaw + (1 - pid->filterCoeff) * pid->dTemp;

    pid->dTerm = pid->kd * pid->dTemp;

    // Compute final output
    pid->u0 = pid->pTerm + pid->iTerm + pid->dTerm;

    // Safety shut-off for large angles
    if (fabs(angle) > 37.0) {
        pid->u0 = 0.0;
    }

    // Output saturation (limit to -1 to 1)
    if (pid->u0 > MAX_PIDOUT) pid->u0 = MAX_PIDOUT;
    else if (pid->u0 < MIN_PIDOUT) pid->u0 = MIN_PIDOUT;
}



float getOutputPID(PID_t *pid)
{
    
    return pid->u0;
}

