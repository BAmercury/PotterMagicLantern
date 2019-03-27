#include <Adafruit_NeoPixel.h>
#include "Colors.h"

#define PIN_LED_DATA 8
#define PIN_SENSOR_INPUT 2

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN_LED_DATA, NEO_GRB + NEO_KHZ800);




void setup()
{
  Serial.begin(115200);
  strip.begin();
  strip.show();
}



void loop()
{
  //animate_led();
    // Some example procedures showing how to display to the pixels:
  colorWipe(strip.Color(255, 0, 0), 50); // Red
  colorWipe(strip.Color(0, 255, 0), 50); // Green
  colorWipe(strip.Color(0, 0, 255), 50); // Blue
  // Send a theater pixel chase in...
  theaterChase(strip.Color(127, 127, 127), 50); // White
  theaterChase(strip.Color(127,   0,   0), 50); // Red
  theaterChase(strip.Color(  0,   0, 127), 50); // Blue

  rainbow(20);
  rainbowCycle(20);
  theaterChaseRainbow(50);
}
