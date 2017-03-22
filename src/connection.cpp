#include "debug.h"
#include "connection.h"

Connection::Connection() :
    WiFiMulti(new ESP8266WiFiMulti), netMode(0), rosca(false)
{
}

void Connection::addWifi(String ssid, String password = "")
{
    rosca = true;
    if(password.length() != 0)
        WiFiMulti->addAP(ssid.c_str(), password.c_str());
    else
        WiFiMulti->addAP(ssid.c_str());
}

ESP8266WiFiClass* Connection::wifi()
{
    return &WiFi;
}

bool Connection::getStatus()
{
    return WiFiMulti->run() == WL_CONNECTED;
}

bool Connection::run()
{
    data->networkName = wifi()->SSID().c_str();
    data->macAddress = wifi()->macAddress().c_str();
    data->hostName = wifi()->hostname().c_str();
    data->networkIP = wifi()->localIP().toString().c_str();
    data->RSSI = wifi()->RSSI();
    data->BSSID = wifi()->BSSIDstr().c_str();
    netMode = 0;
    if(rosca)
        return getStatus();
    else
        return false;
}

void Connection::setData(dataStruct* d)
{
    data = d;
}

bool Connection::scanForNetwork(const char *ssid){
    int n = WiFi.scanNetworks();
    debug("Scan Done");
    if (n == 0)
        Serial.println("no networks found");
    else {
        for (int i = 0; i < n; ++i){
            if(WiFi.SSID(i) == ssid){
                netMode = 1;
            }
        }
    }
    if(netMode == 1)
        return true;
    else
        return false;
}

void setupAP(const char *ssid, const char *password){

}

Connection& Connection::self()
{
    static Connection self;
    return self;
}

Connection::~Connection()
{
};