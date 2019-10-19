// Quick script to pull sensor data and study it's noise frequencies to improve filter desiogn
#include <TimerOne.h>


int PIN_ADC_IN = A2;

volatile int val;


void isr()
{
    val = analogRead(A2);
}


void setup()
{
    Timer1.initialize(1000);
    Timer1.attachInterrupt(isr);
    Serial.begin(115200);

}

void loop()
{
    int rd;

    noInterrupts();
    rd = val;
    interrupts();

    Serial.println(rd);
    delay(5);

}
