#include <oled.h>

void oLedSetup()
{
    Serial.begin(BAUD);
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }

    // Show initial display buffer contents on the screen --
    // the library initializes this with an Adafruit splash screen.
    display.display();
    delay(2000); // Pause for 2 seconds
    
    // Clear the buffer
    display.clearDisplay();
}
void oLedLoop()
{
    float d_angle = getDesiredAngle(&pid);

    display.clearDisplay();

    display.setTextSize(1);             // Normal 1:1 pixel scale
    display.setTextColor(SSD1306_WHITE);        // Draw white text
    display.setCursor(0,0);             // Start at top-left corner
    display.print(F("Desired Angle: "));
    display.println(d_angle);

    display.display();
    delay(2000);
}
