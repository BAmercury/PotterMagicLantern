#define PIN_LED_OUTPUT 6
#define PIN_SENSOR_INPUT 2



bool state = false;

void setup()
{
    pinMode(PIN_LED_OUTPUT, OUTPUT);
    pinMode(PIN_SENSOR_INPUT, INPUT);

    attachInterrupt(PIN_SENSOR_INPUT, input_state_change, RISING);

}



void loop()
{

}

void input_state_change()
{
    state = !state;
    if (state)
    {
        digitalWrite(PIN_LED_OUTPUT, HIGH);
    }
    else
    {
        digitalWrite(PIN_LED_OUTPUT, LOW);
    }
    

}