#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

// Define Wifi Parameters:

#define WIFI_SSD "SSID"
#define WIFI_PASS "Pass"

#define MQTT_SERV "io.adafruit.com"
#define MQTT_PORT 1883
#define MQTT_NAME "Adafruit Username"
#define MQTT_PASS "Adafruit IO Key"



// Create a WifiClient and Adafruit MQTT Client 
WiFiClient client;
Adafruit_MQTT_Client mqtt(&client, MQTT_SERV, MQTT_PORT, MQTT_NAME, MQTT_PASS);
Adafruit_MQTT_Subscribe onoff = Adafruit_MQTT_Subscribe(&mqtt, MQTT_NAME "/f/potterlamponoff");

void MQTT_connect() 
{
  int8_t ret;
  // Stop if already connected
  if (mqtt.connected())
  {
    return;
  }

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) // connect will return 0 for connected
  { 
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) 
    {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
}

void setup()
{
    // Start serial
    Serial.begin(9600);
    // Arduino that is connected should read it's serial bus and use messages to prime the lamp

    // Connect to the Wifi
    WiFi.begin(WIFI_SSD, WIFI_PASS);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
    }

    // Subscribe to the onoff Topic
    mqtt.subscribe(&onoff);
}



void loop()
{
    //Connect/Reconnect to MQTT
  MQTT_connect();

  //Read from our subscription queue until we run out, or
  //wait up to 5 seconds for subscription to update
  Adafruit_MQTT_Subscribe * subscription;
  while ((subscription = mqtt.readSubscription(5000)))
  {
    //If we're in here, a subscription updated...
    if (subscription == &onoff)
    {
      //Print the new value to the serial monitor
      Serial.println((char*) onoff.lastread);

      //If the new value is  "ON", turn the light on.
      //Otherwise, turn it off.
      if (!strcmp((char*) onoff.lastread, "ON"))
      {
        //active low logic
        //digitalWrite(LED_BUILTIN, LOW);
      }
      else
      {
        //digitalWrite(LED_BUILTIN, HIGH);
      }
    }
  }

  // ping the server to keep the mqtt connection alive<br>  if (!mqtt.ping())<br>  {<br>    mqtt.disconnect();<br>  }<br>}
}

