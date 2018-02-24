/*
   Library to Interface with IoT server AERLab over MQTT
   Date: Feb 24, 2017
   Author: Arthur Bondar
   Email: arthur.bondar.1@gmail.com
*/


#ifndef AERClient_h
#define AERClient_h

#include <ESP8266WiFi.h>          // ESP WiFi Libarary
#include <PubSubClient.h>         // MQTT publisher/subscriber client 
#include <stdio.h>
#include "Arduino.h"

/*
    API for interfacing with the server
    Uses MQTT as underlining protocol
*/
class AERClient
{
  public:
    AERClient(int);
    ~AERClient();
    void init(const char*, const char*);
    bool publish(String, String);
    void debug();
    char* mqtt_user = "aerlab";
    char* mqtt_pswd = "server";
    char* _server = "aerlab.ddns.net";
    uint8_t _ID;
    int _port = 1883;
  private:
    const char* _ssid;            // Wifi Name
    const char* _password;        // Wifi Password
    WiFiClient _espClient;        // Wifi library object
    PubSubClient* _client;
    String macToStr(const uint8_t* mac);
    void reconnect();
    String clientName;
};

#endif