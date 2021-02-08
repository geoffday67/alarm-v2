#ifndef __NETWORK_MANAGER__
#define __NETWORK_MANAGER__

#include "ESP8266WiFi.h"
#include <WiFiUdp.h>
#include <PubSubClient.h>

class classNetworkManager {
private:
    char ssid[32];
    char password[32];
    uint8_t mac[6];
    bool enabled;
    WiFiClient wifi;
    PubSubClient mqtt;
    WiFiUDP ntpUDP;
    bool ntpInProgress = false;
    
    bool connectWiFi();
    void disconnectWiFi();
    
public:
    classNetworkManager();
    void setSSID(const char* pssid, const char *ppassword);
    void getSSID(char *pssid);
    bool getEnabled();
    void enable();
    void disable();
    void kettleOn();
    void syncTime();
};

extern classNetworkManager NetworkManager;

#endif