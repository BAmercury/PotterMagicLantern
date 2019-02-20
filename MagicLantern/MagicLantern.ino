#include <Adafruit_NeoPixel.h>
#include "Colors.h"

#define PIN_LED_DATA 3


//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN_LED_DATA, NEO_GRB + NEO_KHZ800);
//Tweak these max color values for white balance (0-255)
#define RED_MAX 255
#define GREEN_MAX 255
#define BLUE_MAX 255



void hslToRgb(float h, float s, float l, uint8_t * rgbOut)
{
  //Read hue offset from potentiometer
  int huePotValue = 1023;
  if (Serial.available() >> 0)
  {
     int huePotValue = Serial.parseInt();
    
  }
 
  float huePotPercent = (float)huePotValue / 1023;

  //Offset hue value
  h += huePotPercent;
  h -= (int)h;

  //Convert HSL to RGB
  float r, g, b;

  if (s == 0.0f)
  {
    r = g = b = l; // achromatic
  }
  else
  {
    float q = (l < 0.5f) ? (l * (1 + s)) : (l + s - l * s);
    float p = 2 * l - q;
    r = hueToRgb(p, q, h + 1.0f / 3.0f);
    g = hueToRgb(p, q, h);
    b = hueToRgb(p, q, h - 1.0f / 3.0f);
  }

  //Apply exponential curve to RGB values for better LED response
  r = r * r * r;
  g = g * g * g;
  b = b * b * b;

  //Apply white balance scales
  rgbOut[0] = (int)(r * RED_MAX);
  rgbOut[1] = (int)(g * GREEN_MAX);
  rgbOut[2] = (int)(b * BLUE_MAX);
}

float hueToRgb(float p, float q, float t)
{
  if (t < 0.0f)
    t += 1.0f;
  if (t > 1.0f)
    t -= 1.0f;
  if (t < 1.0f / 6.0f)
    return p + (q - p) * 6.0f * t;
  if (t < 1.0f / 2.0f)
    return q;
  if (t < 2.0f / 3.0f)
    return p + (q - p) * (2.0f / 3.0f - t) * 6.0f;
  return p;
}

void setup()
{
    Serial.begin(115200);
    strip.begin();
    strip.show(); // Initialize all pixels to 'off'

}

struct light_values
{
  uint8_t red, green blue;
  float hf, sf, lf;
};



void loop()
{
  light_values values;
  for (int i=0; i < sizeof(colors) / 3; ++i)
  {
    values.hf = (float)colors[(i*3)] / 255;
    values.sf = (float)colors[(i*3)+1] / 255;
    values.lf = (float)colors[(i*3)+2] / 255;
    //Read the the next color from progmem
	  //h = pgm_read_byte_near(colors + (i * 3));
	  //s = pgm_read_byte_near(colors + (i * 3) + 1);
	  //l = pgm_read_byte_near(colors + (i * 3) + 2);
    //hf = (float)h / 255;
	  //sf = (float)s / 255;
	  //lf = (float)l / 255;
  }
  


    //Convert HSL color to RGB color
	hslToRgb(hf, sf, lf, rgb);
  Serial.println(rgb[0]);
  Serial.println(rgb[1]);
  Serial.println(rgb[2]);
  for (int i = 0; i <= 8; i++)
  {
     strip.setPixelColor(i, rgb[0], rgb[1], rgb[2]);
  }
  strip.show();
    
  }
   
	//Delay 33 ms for roughly 30FPS
	delay(33);

}


