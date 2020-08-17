#include <Adafruit_NeoPixel.h>
#include "Colors.h"
#include <Smoothed.h>

#define PIN_LED_DATA 8
#define PIN_STATUS_LED 6 // PWM Pin for analogWrite
#define INTERRUPT_PIN_SENSOR_INPUT 2 // piezo circuit digital output
#define INTERRUPT_PIN_ESP_INPUT 3 // ESP32 digital output
#define PIN_POT 1 // Analog input pin 1
#define PIN_MISC_ANIMATE_ENABLE 2 // Analog input pin 2, enables other animations
#define NUM_LED 24

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LED, PIN_LED_DATA, NEO_GRB + NEO_KHZ800);

bool animate = true;

// Animations:
// 0: Fire
// 1: RainbowCycle
// 2: theaterChaseRainbow 
// 3: Wheel
volatile int desired_animation = 0; 


// Potentiometer Sensor
Smoothed <int> PotSensor;
const int WINDOW_SIZE = 10; // Window size for MA filter running for the pot


// Count amount of taps
volatile int tap_count = 0;
// If 2 taps received update this boolean
volatile bool two_taps = false;

void sensor_tap_counter()
{
    tap_count++;
    // If we received two taps, update the control boolean for the main loop
    if (tap_count > 2)
    {
        two_taps = true; // This boolean latches and does not get reset until augamenti is spoken
        // Reset tap counter
        tap_count = 0;
    }

    // Update desired animation based on taps if otherAnimations are enabled
    // also if twoTaps have already been received and the spell is spoken
    analogRead(PIN_MISC_ANIMATE_ENABLE); // discard
    int adc_val = analogRead(PIN_MISC_ANIMATE_ENABLE);
    
    // if we have already tapped twice and the animation is running
    if (two_taps && adc_val >= 200)
    {
        switch (tap_count)
        {
            case 0:
                desired_animation = 1; // RainbowCycle
                break;
            case 1:
                desired_animation = 2; // theaterChaseRainbow
                break;
            case 2:
                desired_animation = 3; // Wheel
                break;
        }
    }
    else if(two_taps)
    {
        desired_animation = 0; // Fire
    }
    

}

volatile bool esp_state = false;
void esp_state_change()
{
    // Flip the control boolean for every detected state change
    esp_state = !esp_state;
}



void setup()
{
    Serial.begin(115200);
    strip.begin();
    strip.show();
    pinMode(PIN_STATUS_LED, OUTPUT);
    pinMode(INTERRUPT_PIN_SENSOR_INPUT, INPUT);
    pinMode(INTERRUPT_PIN_ESP_INPUT, INPUT);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_SENSOR_INPUT), sensor_tap_counter, RISING);
    attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN_ESP_INPUT), esp_state_change, RISING);

    // initialize the Pot Sensor
    PotSensor.begin(SMOOTHED_AVERAGE, WINDOW_SIZE);
}



void loop()
{

    // Check Serial Buffer

    // Disable interrupt so we can check our booleans
    noInterrupts();
    // Quickly copy the control booleans
    bool esp_state_cpy = esp_state;
    bool two_taps_cpy = two_taps;
    int desired_animation_cpy = desired_animation;
    interrupts(); // Reenable interrupts

    // Check boolean logic
    // We want this to latch, so incease two_taps_cpy resets for any reason we can still run the animation
    // if esp_state_cpy is false (Only happens if we speak aguamenti - the animation should turn off right away)

    // let user know we recieved two taps and waiting for spell
    if (two_taps_cpy && esp_state_cpy == false)
    {
        digitalWrite(PIN_STATUS_LED, HIGH);
    }

    // If we received two hits and we have spoken Incendio
    if (two_taps_cpy && esp_state_cpy) 
    {
        animate = true; 
    }
    // If we have spoken the aguamenti spell, turn off animation
    if (esp_state_cpy == false && animate)
    {
        Serial.println("Turning off animation");
        animate = false;
        digitalWrite(PIN_STATUS_LED, LOW);
        noInterrupts();
        // Reset global variable
        two_taps = 0;
        interrupts();
    }


    // Override for debugging
    //bool animate = true;
    if (animate)
    {
        Serial.println(desired_animation_cpy);
        switch (desired_animation_cpy)
        {
            case 0:
                // Runs fire animation
                animate_led();
                break;
            case 1:
                rainbowCycle(10);
                break;
            case 2:
                theaterChaseRainbow(10);
                break;
            case 3:
                rainbow(10);
                break;
        }
    }
    else
    {
        // Turn off all LEDs
        for (int j = 0; j <=NUM_LED; j++)
        {
            strip.setPixelColor(j, 0,0,0);
        }
        strip.show();
        // Turn off status LED
        delay(10);
    }



}
