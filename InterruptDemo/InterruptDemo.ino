#define PIN_LED_OUTPUT 6
#define PIN_SENSOR_INPUT 2



volatile bool state = false;

void setup()
{
    Serial.begin(115200);
    pinMode(PIN_LED_OUTPUT, OUTPUT);
    pinMode(PIN_SENSOR_INPUT, INPUT);
    attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_INPUT), input_state_change, RISING);

}



void loop()
{

}

void input_state_change()
{
   
    if (state)
    {
        
        digitalWrite(PIN_LED_OUTPUT, HIGH);
        state = false;
    }
    else
    {
        
        digitalWrite(PIN_LED_OUTPUT, LOW);
        state = true;
    }
    

}