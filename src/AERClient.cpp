
#include <AERClient.h>            // Library for One-Wire interface

// Constructor that that stores Module ID
AERClient::AERClient(int ID)
{
    _ID = ID;
}

// Constructor that that stores Module ID and timeout value
AERClient::AERClient(int ID, unsigned int timeout)
{
    _ID = ID;
    _timeout = timeout;
}

/*
 Establishes connection to WiFi access point with parameters provided
 Connects to MQTT broker - aerlab.ddns.net
 */
bool AERClient::init(const char* ssid, const char* password)
{
    uint8_t mac[6], status;
    // Saving Wifi Parametrs
    _ssid = ssid;
    _password = password;
    // Creating PubSubClient Object
    _client = new PubSubClient(_server, _port, _espClient);
    _client->setServer(_server, 1883);
    // Generating ID for MQTT broker
    WiFi.macAddress(mac);
    clientName = "esp8266-" + macToStr(mac) + "-" + String(_ID);
    // Establish WiFi Connection
    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    if(!wifiConnect()) {
        return false;
    }
    //check for MQTT Connection
    if (!_client->connected()) reconnect();
    _client->loop();
    return true;
}

// Destructor to de-allocate memory
AERClient::~AERClient()
{
    delete _client;
}

void AERClient::disableReconnect() {
    _reconnectFlag = false;
}

void AERClient::enableReconnect() {
    _reconnectFlag = true;
}

/*
 Published Data to AERLab server
 First parameter - Topic, second - Payload
 */
bool AERClient::publish(String topic, String payload)
{
    bool _result = false;
    if (WiFi.localIP().toString() != "0.0.0.0")
    {
        reconnect();
        _client->loop();
        _result = _client->publish((String(_ID) + "/" + topic).c_str(), payload.c_str(), 2);
    }
    else
    {
        if(!wifiConnect()) {
            return false;
        }
        reconnect();                    //check for MQTT Connection
        _client->loop();
    }
    return _result;
}

/*
 Subscribe to an MQTT topic
 First parameter - topic, Second parameter - pointer to callback function
 */
void AERClient::subscribe(char* topic, void (*pCallback)(char*, byte*, unsigned int)) {
    _client->subscribe(topic);
    _client->setCallback(pCallback);
}

/*
 Connect to Wi-Fi access point
 */
bool AERClient::wifiConnect()
{
    // Attempt to connect to access point
    WiFi.begin(_ssid, _password);
    uint8_t i = 0;
    // Wait for Wi-Fi to connect or for timeout
    while (WiFi.localIP().toString() == "0.0.0.0" && i <= _timeout && _reconnectFlag)
    {
        delay(1000);
        Serial.print(".");
        i++;
    }
    if(i > _timeout || !_reconnectFlag)
    {
        return false;  // Could not connect
    }
    return true;
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
            delay(10 * 1000);
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

void AERClient::loop() {
    if (!_client->connected()) {
        wifiConnect();
    }
    _client->loop();
}

/*
 Used to Debug library operation
 Requeires Serial.begin()
 */
void AERClient::debug()
{
    Serial.println("\n------- AERClient DEBUG ------- ");
    Serial.print("WiFi IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println("Module Name: " + clientName);
    Serial.println("Connecting to: " + String(_ssid));
    Serial.println("With password: " + String(_password));
    Serial.println("MQTT Status: " + String(_client->state()));
    Serial.println("------- WiFi DEBUG ------- ");
    WiFi.printDiag(Serial);
}


