#include <actuator.h>
#include <Servo.h>

// Servo objects and pin definitions
Servo myservo1;
Servo myservo2;
const uint8_t servoLeft = A0;
const uint8_t servoRight = A1;

// Actuator state management
enum class PlatformState { IDLE, EXTENDING, SHORTENING };
PlatformState platformState = PlatformState::IDLE;
int currentPos1 = 0;
int currentPos2 = 180;
unsigned long lastServoUpdate = 0;
const uint8_t SERVO_UPDATE_INTERVAL = 1; // ms

void actuatorSetup() {
    Serial.begin(BAUD);
    
    // Initialize servos with verification
    bool servo1Attached = myservo1.attach(servoLeft);
    bool servo2Attached = myservo2.attach(servoRight);
    
    // Initial position
    myservo1.write(currentPos1);
    myservo2.write(currentPos2);
    
    // Debug output
    Serial.println(servo1Attached ? "Actuator 1 attached" : "Actuator 1 FAILED");
    Serial.println(servo2Attached ? "Actuator 2 attached" : "Actuator 2 FAILED");
}

void updateServoPositions() {
    myservo1.write(currentPos1);
    myservo2.write(currentPos2);
    Serial.print(currentPos1);
    Serial.print(" ");
    Serial.println(currentPos2);
}

void actuatorLoop(const char* command) {
    // Handle new commands
    if (strcmp(command, "EXTEND_PLATFORM") == 0) {
        platformState = PlatformState::EXTENDING;
    } 
    else if (strcmp(command, "SHORTEN_PLATFORM") == 0) {
        platformState = PlatformState::SHORTENING;
    }
    else if (strcmp(command, "STOP_PLATFORM") == 0) {
        platformState = PlatformState::IDLE;
    }

    // Non-blocking position update
    if (platformState != PlatformState::IDLE && 
        millis() - lastServoUpdate >= SERVO_UPDATE_INTERVAL) {
        
        lastServoUpdate = millis();
        
        switch (platformState) {
            case PlatformState::EXTENDING:
                if (currentPos1 < 180 && currentPos2 > 0) {
                    currentPos1++;
                    currentPos2--;
                    updateServoPositions();
                } else {
                    platformState = PlatformState::IDLE;
                }
                break;
                
            case PlatformState::SHORTENING:
                if (currentPos1 > 0 && currentPos2 < 180) {
                    currentPos1--;
                    currentPos2++;
                    updateServoPositions();
                } else {
                    platformState = PlatformState::IDLE;
                }
                break;
                
            case PlatformState::IDLE:
                // No action needed
                break;
        }
    }
}
