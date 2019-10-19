#include <Adafruit_NeoPixel.h>
#include "Colors.h"

#define PIN_LED_DATA 8
#define PIN_SENSOR_INPUT 2
#define NUM_LED 24

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LED, PIN_LED_DATA, NEO_GRB + NEO_KHZ800);

volatile int touch_count = 0;
volatile bool animation = false;
// Animations:
// 0: Fire
// 1: RainbowCycle
// 2: theaterChaseRainbow 
volatile int desired_animation = 0; 


void input_state_change()
{
  touch_count++;

  // Keep LED off if no two touch
  if (touch_count < 2)
  {
    animation = false;
    digitalWrite(LED_BUILTIN, LOW);
  }
  // Turns on LED
  if (touch_count > 2)
  {
    animation = true;
    digitalWrite(LED_BUILTIN, HIGH);
    touch_count = 0;

  }

 

}



void setup()
{
  Serial.begin(115200);
  strip.begin();
  strip.show();
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(PIN_SENSOR_INPUT, INPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_INPUT), input_state_change, RISING);
}



void loop()
{

  // Check Serial Buffer

  // Disable interrupt so we can check boolean
  noInterrupts();
  // Quickly copy the control boolean
  bool animate = true;

  if (animate)
  {

    switch (desired_animation)
    {
      case 0:
        animate_led();
        break;
      case 1:
        rainbowCycle(10);
        break;
      case 2:
        theaterChaseRainbow(10);
        break;
    }
  }
  else
  {
    for (int j = 0; j <=NUM_LED; j++)
    {
      strip.setPixelColor(j, 0,0,0);
    }

    strip.show();
    
    
  }
  // Start the interrupt again
  interrupts();
  

}
