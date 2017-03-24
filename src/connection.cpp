#include "debug.h"
#include "connection.h"

Connection::Connection() :
    WiFiMulti(new ESP8266WiFiMulti), isMaster(false), wifi_added(false)
{
}

void Connection::addWifi(String ssid, String password = String())
{
    wifi_added = true;
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
    isMaster = false;
    if(wifi_added)
        return getStatus();
    return false;
}

void Connection::setData(dataStruct* d)
{
    data = d;
}

bool Connection::scanForNetwork(const char *ssid){
    int numRedes = WiFi.scanNetworks();
    debug("Scan Done");
    if(numRedes != 0) {
        for (int i = 0; i < numRedes; i++){
            if(WiFi.SSID(i) == ssid){
                isMaster = 1;
            }
        }
    }
    if(isMaster)
        return (true);
    return false;
}

Connection& Connection::self()
{
    static Connection self;
    return self;
}

Connection::~Connection()
{
};