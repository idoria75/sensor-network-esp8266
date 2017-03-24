#pragma once

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClient.h>
#include "data.h"

class Connection
{
public:

    void addWifi(String ssid, String password);

    ESP8266WiFiClass* wifi();

    bool run();

    void setData(dataStruct* d);

    bool getStatus();

    static Connection& self();
    ~Connection();

    bool scanForNetwork(const char *ssid);

private:
    Connection& operator = (Connection& other) = delete;
    Connection(const Connection& other) = delete;
    Connection();

    ESP8266WiFiMulti* WiFiMulti;
    dataStruct* data;
    bool wifi_added;

    bool isMaster;
};