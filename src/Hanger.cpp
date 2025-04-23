/* 
 * Project myProject
 * Author: Your Name
 * Date: 
 */

#include "Particle.h"
#include "../lib/neopixel/src/neopixel.h"

#define PIXEL_COUNT 12
#define PIXEL_PIN SPI1
#define PIXEL_TYPE WS2812
#define LIGHT_THRESHOLD 2000


int sensorPin = A0;

Adafruit_NeoPixel ring(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
bool isRingOn = false;
bool isRingOff = false;

int brightness = 0;
int fadeAmount = 1;
unsigned long lastUpdate = 0;
const int breatheDelay = 12; // Controls breathing speed

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

void turnRingOn() {
    isRingOn = true;
    isRingOff = false;
    brightness = 50; // Start at middle brightness
}

void turnRingOff() {
    for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, 0, 0, 0);
    }
    ring.show();
    isRingOn = false;
    isRingOff = true;

}

// Cloud function handlers - these match the reference code pattern
int turnOnHandler(String cmd) {
    turnRingOn();
    return 0;
}

int turnOffHandler(String cmd) {
    turnRingOff();
    return 0;
}

int toggleHandler(String cmd) {
    if (isRingOn) {
        turnRingOff();
    } else {
        turnRingOn();
    }
    return 0;
}

void setup() {
    ring.begin();
    pinMode(sensorPin, INPUT);
    turnRingOn();  // Initialize ring to on state
    
    // Expose functions to the Particle Cloud
    Particle.function("turnOn", turnOnHandler);
    Particle.function("turnOff", turnOffHandler);
    Particle.function("toggle", toggleHandler);
}

void loop() {
    int reading = analogRead(sensorPin);
    Serial.print("Light level is: ");
    Serial.println(reading);

    
    if (reading >= LIGHT_THRESHOLD && isRingOn) {
        turnRingOff();
        isRingOff = true;
    }

    if (reading < LIGHT_THRESHOLD && isRingOff) {
        turnRingOn();
        isRingOn = true;
    }

    if (isRingOn) {
        unsigned long currentTime = millis();
        
        // Update brightness every breatheDelay milliseconds
        if (currentTime - lastUpdate > breatheDelay) {
            // Change brightness
            brightness = brightness + fadeAmount;
            
            // Reverse direction at the limits
            if (brightness <= 0 || brightness >= 100) {
                fadeAmount = -fadeAmount;
            }
            
            // Update all pixels
            for (int i = 0; i < ring.numPixels(); i++) {
                ring.setPixelColor(i, 86, 199, 255); // Red color
            }
            
            ring.setBrightness(brightness);
            ring.show();
            
            lastUpdate = currentTime;
        }
    }
}