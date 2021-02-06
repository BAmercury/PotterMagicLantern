//Tweak these max color values for white balance (0-255)
#define RED_MAX 255
#define GREEN_MAX 100
#define BLUE_MAX 100


// Hue offset, can be changed through attached potentiometer
// default is red
int int_hue = 899;
int index_value = 0; // Track through the color array using this

struct hsl_values
{
    float h, s, l;
};

struct rgb_values
{
    int red, blue, green;
};

uint8_t h, s, l;
hsl_values hsl;      

// Function to Convert HSL to RGB, incorporate hue shift
struct rgb_values hsl2RGB(struct hsl_val hsl_val);
struct rgb_values hsl2RGB(struct hsl_values hsl_val)
{
    //  Get Hue offset from the potentiometer then run through MA filter
    analogRead(PIN_POT); // discard
    int_hue = analogRead(PIN_POT);
    // Add new value then read the smoothed result
    PotSensor.add(int_hue);
    //int_hue = PotSensor.get();

    // Evan Kale: https://github.com/evankale
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


// Read animation files from Colors.h and convert to RGB. Output to Neopixel strip
void animate_led()
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

    for (int j = 0; j <= NUM_LED; j++)
    {
        strip.setPixelColor(j, rgb.red, rgb.blue, rgb.green);
    }
    strip.show();
    // Updates every 33 ms (Can use this to control framerate of animation)
    //delay(33);
  }

}

rgb_values get_next_hsl_values(int i)
{
    // Tracks through colors array (Colors.h) and returns rgb values 

    // Get next color from progmem
    h = pgm_read_byte_near(colors + (i * 3));
    s = pgm_read_byte_near(colors + (i * 3) + 1);
    l = pgm_read_byte_near(colors + (i * 3) + 2);

    // Convert to float and rescale
    hsl.h = (float)h / 255;
    hsl.s = (float)s / 255;
    hsl.l = (float)l / 255;
    // Convert to RGB
    rgb_values rgb;
    rgb = hsl2RGB(hsl);

    return rgb;
}

void animate_led_interval()
{
    // Get RGB values from Colors.h
    rgb_values rgb = get_next_hsl_values(index_value);
    // Set Pixel Colours
    for (int j = 0; j <= NUM_LED; j++)
    {
    strip.setPixelColor(j, rgb.red, rgb.blue, rgb.green);
    }
    strip.show();
    // If we reached the end of the color array, set index_value back to 0
    if (index_value == sizeof(colors) / 3)
    {
    index_value = 0; 
    }
    else
    {
    // Otherwise update iterator
    index_value++;
    }

}