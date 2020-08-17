// Adafruit Neopixel Demo Code
// https://github.com/ndsh/neopixel-without-delay

int theaterChaseQ = 0;
int theaterChaseRainbowQ = 0;
int theaterChaseRainbowCycles = 0;
int rainbowCycles = 0;
int rainbowCycleCycles = 0;

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle() 
{
    uint16_t i;
    for(i=0; i< strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + rainbowCycleCycles) & 255));
    }
    strip.show();
    rainbowCycleCycles++;
    if(rainbowCycleCycles >= 256*5) rainbowCycleCycles = 0;
}

void rainbow() 
{
    for(uint16_t i=0; i<strip.numPixels(); i++) 
    {
        strip.setPixelColor(i, Wheel((i+rainbowCycles) & 255));
    }
    strip.show();
    rainbowCycles++;
    if(rainbowCycles >= 256) rainbowCycles = 0;
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow() 
{
    for (int i=0; i < strip.numPixels(); i=i+3) 
    {
        strip.setPixelColor(i+theaterChaseRainbowQ, Wheel( (i+theaterChaseRainbowCycles) % 255));    //turn every third pixel on
    }
      
    strip.show();
    for (int i=0; i < strip.numPixels(); i=i+3) 
    {
        strip.setPixelColor(i+theaterChaseRainbowQ, 0);        //turn every third pixel off        
    }      
    theaterChaseRainbowQ++;
    theaterChaseRainbowCycles++;
    if(theaterChaseRainbowQ >= 3) theaterChaseRainbowQ = 0;
    if(theaterChaseRainbowCycles >= 256) theaterChaseRainbowCycles = 0;
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) 
{
    WheelPos = 255 - WheelPos;
    if (WheelPos < 85) 
    {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    } 
    else if (WheelPos < 170) 
    {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }  
    else 
    {
        WheelPos -= 170;
        return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}


