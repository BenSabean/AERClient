
#include "AERClient.h"                // Library for One-Wire interface

// Constructor that that stores Module ID
AERClient::AERClient(int ID)
{
  _ID = ID;
}

/*
  Establishes connection to WiFi access point with parameters provided
  Connects to MQTT broker - aerlab.ddns.net
*/
void AERClient::init(const char* ssid, const char* password)
{
  uint8_t mac[6];

  // Saving Wifi Parametrs
  _ssid = ssid;
  _password = password;
  // Creating PubSubClient Object
  _client = new PubSubClient(_server, _port, _espClient);
  _client->setServer(_server, 1883);
  // Generating ID for MQTT broker
  clientName += "esp8266-";
  WiFi.macAddress(mac);
  clientName += macToStr(mac);
  clientName += "-";
  clientName += String(_ID);
  // Establish WiFi Connection
  WiFi.begin(_ssid, _password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }
  //check for MQTT Connection
  if (!_client->connected()) reconnect();
  _client->loop();
}

// Destructor to de-allocate memory
AERClient::~AERClient()
{
  delete _client;
}

/*
  Published Data to AERLab server
  First parameter - Topic, second - Payload
*/
bool AERClient::publish(String topic, String payload)
{
  bool _result = false;
  if (WiFi.status() == WL_CONNECTED)
    _result = _client->publish((String(_ID) + "/" + topic).c_str(), payload.c_str());
  else
  {
    WiFi.begin(_ssid, _password);   // WiFi reconnect 
    reconnect();                    //check for MQTT Connection
    _client->loop();
  }
  return _result;
}

/*
  Reconnecting to MQTT broker
*/
void AERClient::reconnect()
{
  // Loop until we're reconnected
  while (!_client->connected()) 
  {
    if (!_client->connect(clientName.c_str(), mqtt_user, mqtt_pswd))
      delay(30000);
  }
}

/*
  MAC address used as MQTT identification
*/
String AERClient::macToStr(const uint8_t* mac)
{
  String result;
  for (int i = 0; i < 6; ++i) {
    result += String(mac[i], 16);
    if (i < 5)
      result += ':';
  }
  return result;
}

/*
  Used to Debug library operation
  Requeires Serial.begin()
*/
void AERClient::debug()
{
  Serial.println(" ------- AERClient DEBUG ------- ");

  Serial.print("WiFi IP address: ");
  Serial.println(WiFi.localIP());

  Serial.println("Module Name: " + clientName);
  Serial.println("Connecting to: " + String(_ssid));
  Serial.print("With password: " + String(_password));

  Serial.println("MQTT Status: " + String(_client->state()));
}


