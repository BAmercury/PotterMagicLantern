#include <Adafruit_NeoPixel.h>
#include "Colors.h"

#define PIN_LED_DATA 3


//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN_LED_DATA, NEO_GRB + NEO_KHZ800);
//Tweak these max color values for white balance (0-255)
#define RED_MAX 255
#define GREEN_MAX 100
#define BLUE_MAX 100

int int_hue = 899;

struct hsl_values
{
  float h, s, l;
};

struct rgb_values
{
  int red, blue, green;
};

struct rgb_values hsl2RGB(struct hsl_val hsl_val);
struct rgb_values hsl2RGB(struct hsl_values hsl_val)
{
  // Hue offset
  if (Serial.available() > 0)
  {
    int_hue = Serial.parseInt();
  }

  float huePotPercent = (float)int_hue / 1023;

  // Add the offset
  hsl_val.h += huePotPercent;
  hsl_val.h -= (int)hsl_val.h;

  //Convert HSL to RGB
  float r, g, b;


  if (hsl_val.s == 0.0f)
  {
    r = g = b = hsl_val.l; // achromatic
  }
  else
  {
    float q = (hsl_val.l < 0.5f) ? (hsl_val.l * (1 + hsl_val.s)) : (hsl_val.l + hsl_val.s - hsl_val.l * hsl_val.s);
    float p = 2 * hsl_val.l - q;
    r = hueToRgb(p, q, hsl_val.h + 1.0f / 3.0f);
    g = hueToRgb(p, q, hsl_val.h);
    b = hueToRgb(p, q, hsl_val.h - 1.0f / 3.0f);

  }

  //Apply exponential curve to RGB values for better LED response
  r = r * r * r;
  g = g * g * g;
  b = b * b * b;

  rgb_values rgb;
  //Apply white balance scales
  rgb.red = (int)(r * RED_MAX);
  rgb.green = (int)(g * GREEN_MAX);
  rgb.blue = (int)(b * BLUE_MAX);
  Serial.print(rgb.red);
  Serial.print(',');
  Serial.print(rgb.green);
  Serial.print(',');
  Serial.println(rgb.blue);
  return rgb;
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
  strip.show();

}



void loop()
{

  uint8_t h, s, l;
  hsl_values hsl;
  for (int i = 0; i < sizeof(colors) / 3; ++i)
  {
    //Read the the next color from progmem
		h = pgm_read_byte_near(colors + (i * 3));
		s = pgm_read_byte_near(colors + (i * 3) + 1);
		l = pgm_read_byte_near(colors + (i * 3) + 2);

    hsl.h = (float)h / 255;
    hsl.s = (float)s / 255;
    hsl.l = (float)l / 255;
    rgb_values rgb;
    rgb = hsl2RGB(hsl);

    for (int j = 0; j <= 8; j++)
    {
      strip.setPixelColor(j, rgb.red, rgb.blue, rgb.green);
    }

    strip.show();

    delay(33);



  }

}
