/***************************************************  
Potter Lamp ESP-01 Module Code
Connects to Wifi and checks updates from Adafruit IO feed that is toggled using Google Assistant
References the following definitions from a config.h header file:
  WLAN_SSID
  WLAN_PASS
  AIO_SERVER
  AIO_SERVERPORT
  AIO_USERNAME
  AIO_KEY

  
Referenced code from:
  Tony DiCola for Adafruit Industries
 ****************************************************/
#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "config.h"



// Create ESP8266 WifiClient class
WiFiClient client;

// Setup MQTT client, make sure to include config.h for credientials
Adafruit_MQTT_Client mqtt(&client, AIO_SERVER, AIO_SERVERPORT, AIO_USERNAME, AIO_KEY);


// Setup subscriber feeds
// MQTT Paths for Adafruit IO follow the form: <username>/feeds/<feedname>
Adafruit_MQTT_Subscribe spell_toggle = Adafruit_MQTT_Subscribe(&mqtt, AIO_USERNAME "/feeds/potterlamponoff");






// Bug workaround for Arduino 1.6.6, it seems to need a function declaration
// for some reason (only affects ESP8266, likely an arduino-builder bug).
void MQTT_connect();

void setup() 
{

  // Setup Serial
  Serial.begin(115200);
  delay(10);

  // Setup Pins (GPIO2)
  pinMode(2, OUTPUT);


  // Connect to Wifi Access Point
  Serial.println("Connecting to WIFI");
  // Connect to WiFi access point.
  WiFi.begin(WLAN_SSID, WLAN_PASS);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: "); Serial.println(WiFi.localIP());

  // Setup MQTT subscription for the lamp toggle feed
  mqtt.subscribe(&spell_toggle);

}

uint32_t x=0;

void loop() {
  // Ensure the connection to the MQTT server is alive (this will make the first
  // connection and automatically reconnect when disconnected).  See the MQTT_connect
  // function definition further below.
  MQTT_connect();

  // wait for incoming packets
  Adafruit_MQTT_Subscribe *subscription;
  while ((subscription = mqtt.readSubscription(5000))) 
  {
    if (subscription == &spell_toggle) 
    {

      Serial.println((char *)spell_toggle.lastread);
      // Toggle LED depending on status of the packet
      if ( !strcmp((char*) spell_toggle.lastread, "ON") )
      {
        // Set the GPIO2 Pin High
        digitalWrite(2, HIGH);
      }
      else
      {
        digitalWrite(2, LOW);
      }      
    }
  }
  // ping the server to keep the mqtt connection alive
  if(! mqtt.ping()) {
    mqtt.disconnect();
  }

}

// Function to connect and reconnect as necessary to the MQTT server.
// Should be called in the loop function and it will take care if connecting.
void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
       Serial.println(mqtt.connectErrorString(ret));
       Serial.println("Retrying MQTT connection in 5 seconds...");
       mqtt.disconnect();
       delay(5000);  // wait 5 seconds
       retries--;
       if (retries == 0) {
         // basically die and wait for WDT to reset me
         while (1);
       }
  }
  Serial.println("MQTT Connected!");
}
