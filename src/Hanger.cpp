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

Adafruit_NeoPixel ring(PIXEL_COUNT, PIXEL_PIN, PIXEL_TYPE);
bool isRingOn = false;

SYSTEM_MODE(AUTOMATIC);
SYSTEM_THREAD(ENABLED);
SerialLogHandler logHandler(LOG_LEVEL_INFO);

void turnRingOn() {
    for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, 255, 0, 0);  // Red color
    }
    ring.setBrightness(50);
    ring.show();
    isRingOn = true;
}

void turnRingOff() {
    for (int i = 0; i < ring.numPixels(); i++) {
        ring.setPixelColor(i, 0, 0, 0);
    }
    ring.show();
    isRingOn = false;
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
    turnRingOn();  // Initialize ring to on state
    
    // Expose functions to the Particle Cloud
    Particle.function("turnOn", turnOnHandler);
    Particle.function("turnOff", turnOffHandler);
    Particle.function("toggle", toggleHandler);
}

void loop() {
    // Your loop code here
}