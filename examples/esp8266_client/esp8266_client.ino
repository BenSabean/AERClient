/*
   AERClient library Example Code
   Date: Feb 24, 2018
*/

#include <ESP8266WiFi.h>          // ESP WiFi Libarary
#include <PubSubClient.h>         // MQTT publisher/subscriber client 
#include <AERClient.h>

//////////////////////////////////////
//    CHANGE TO YOUR UNIQUE ID      //
#define DEVICE_ID 5                 //
//                                  //
//////////////////////////////////////

/* Wifi setup */
const char ssid[] =        "WifiName";
const char password[] =    "WifiPassword";

/* Create Library Object password */
AERClient server(DEVICE_ID);

void setup()
{
  Serial.begin(115200);
  /* Initialization and connection to WiFi */
  server.init(ssid, password);
}

void loop()
{
  bool check;
  // Print Debug Info
  server.debug();
  // Publish Data
  check = server.publish("Voltage1", "Hello World");
  Serial.println("Return Code = " + String(check));
  // 5 sec delay
  delay(5000);
}





