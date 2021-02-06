#include <Adafruit_NeoPixel.h>
#include "Colors.h"
#include <Smoothed.h>

#define PIN_LED_DATA 8
#define PIN_STATUS_LED 6 // PWM Pin for analogWrite
#define INTERRUPT_PIN_SENSOR_INPUT 2 // piezo circuit digital output
#define INTERRUPT_PIN_ESP_INPUT 3 // ESP32 digital output
#define PIN_POT 1 // Analog input pin 1
#define NUM_LED 24

//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LED, PIN_LED_DATA, NEO_GRB + NEO_KHZ800);

unsigned long animate_prev_ms = 0;
unsigned long main_loop_prev_ms = 0;
unsigned long fire_interval = 33; // milliseconds
unsigned long rainbow_interval = 10; // milliseconds
unsigned long theater_interval = 50; // milliseconds
unsigned long main_loop_interval = 10; // milliseconds


// Animations:
// 0: Fire
// 1: RainbowCycle
// 2: theaterChaseRainbow 
// 3: Wheel
volatile int desired_animation = 0; 
int desired_animation_cpy = 0;
int prev_desired_animation = 0;
bool animate = true;


// Potentiometer Sensor
Smoothed <int> PotSensor;
const int WINDOW_SIZE = 10; // Window size for MA filter running for the pot


// Count amount of taps
volatile int two_tap_count = 0;
volatile int cycle_mode = 0;
// If 2 taps received update this boolean
volatile bool two_taps = false;
bool two_taps_cpy = false;

void sensor_tap_counter()
{
    two_tap_count++;
    // If we received two taps, update the control boolean for the main loop
    if (two_tap_count > 2)
    {
        two_taps = true; // This boolean latches and does not get reset until augamenti is spoken
        // Reset tap counter
        two_tap_count = 0;
    }

    // if we have already tapped twice assume the animation is running
    if (two_taps)
    {
        if(++cycle_mode > 4) cycle_mode = 0; // Advance to next mode, wrap around after 3
        switch (cycle_mode)
        {
            case 0:
                desired_animation = 0; // Fire
                break;
            case 1:
                desired_animation = 1; // Rainbow Cycle
                break;
            case 2:
                desired_animation = 2; // Theater
                break;
            case 3:
                desired_animation = 3; // Rainbow Solid
        }
    }
    

}

volatile bool esp_state = false;
bool esp_state_cpy = false;
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


    // Run main loop in a programmmable interval
    // Checks status of control bools 
    if ((unsigned long)(millis() - main_loop_prev_ms) >= main_loop_interval)
    {
        main_loop_prev_ms = millis();
        
        // Disable interrupts and copy control booleans
        noInterrupts();
        esp_state_cpy = esp_state;
        two_taps_cpy = two_taps;
        //desired_animation_cpy = desired_animation;
        desired_animation_cpy = 0;
        interrupts(); // Reenable interrupts

        // Now check boolean logic

        // let user know we received two taps and awaiting for incendio spell
        if (animate == false && two_taps_cpy && esp_state_cpy == false)
        {
            digitalWrite(PIN_STATUS_LED, HIGH);
        }
        // If we received two hits and we have spoken the activation spell (incendio)
        if (two_taps_cpy && esp_state_cpy)
        {
            animate = true;
            // Setup the bookeeping timer variables
            animate_prev_ms = 0;
        }
        // If we have spoken the aguamenti spell, turn off the animation and reset variables
        if (animate && esp_state_cpy == false)
        {
            animate = false;
            esp_state_cpy = false;
            two_taps_cpy = false;
            animate_prev_ms = 0;
            digitalWrite(PIN_STATUS_LED, LOW);
            noInterrupts();
            // Reset some global variables
            two_tap_count = 0;
            interrupts();
            // Turn off all LEDs
            for (int j = 0; j <=NUM_LED; j++)
            {
                strip.setPixelColor(j, 0,0,0);
            }
            strip.show();
        }
        
    }
    //animate_led();
    // If the control bools are set, start the animation
    if (true)
    {

        // Select and run the desired animation when the time is appropiate
        switch (desired_animation_cpy)
        {
            case 0:
                if ((unsigned long)(millis() - animate_prev_ms) >= fire_interval)
                {
                    animate_prev_ms = millis();
                    animate_led_interval(); // Run fire animation
                }
                break;
            case 1:
                if ((unsigned long)(millis() - animate_prev_ms) >= rainbow_interval)
                {
                    animate_prev_ms = millis();
                    rainbowCycle(); // Run Rainbow cycle animation
                }
                break;
            case 2:
                if ((unsigned long)(millis() - animate_prev_ms) >= theater_interval)
                {
                    animate_prev_ms = millis();
                    theaterChaseRainbow();
                }
                break;
            case 3:
                if ((unsigned long)(millis() - animate_prev_ms) >= rainbow_interval)
                {
                    animate_prev_ms = millis();
                    rainbow();
                }
                break;
        }
    }



}
