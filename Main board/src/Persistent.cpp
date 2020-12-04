#include <Arduino.h>
#include "Persistent.h"
#include <EEPROM.h>

#define ALARM_ADDRESS           2
#define WIFI_ADDRESS            32

classPersistent Persistent;

classPersistent::classPersistent () {
    EEPROM.begin (256);

    // Check if the EEPROM is initialised
    char signature[3];
    signature[0] = EEPROM.read(0);
    signature[1] = EEPROM.read(1);
    signature[2] = 0;
    if (strcmp(signature, "GD")) {
        // Not initialised, store signature and default values
        Serial.println("Writing default values to EEPROM");
        EEPROM.write(0, 'G');
        EEPROM.write(1, 'D');
        storeAlarm(0, 0, false);

        uint8_t default_mac[] = {0, 0, 0, 0, 0, 0};
        storeWiFi("", "", default_mac, false);
    }
}

struct AlarmSettings {
    int hour;
    int minute;
    bool enabled;
};

void classPersistent::storeAlarm(int hour, int minute, bool enabled) {
    struct AlarmSettings settings;
    settings.hour = hour;
    settings.minute = minute;
    settings.enabled = enabled;
    EEPROM.put(ALARM_ADDRESS, settings);
    EEPROM.commit();
}

void classPersistent::fetchAlarm(int &hour, int &minute, bool &enabled) {
    struct AlarmSettings settings;
    EEPROM.get(ALARM_ADDRESS, settings);
    hour = settings.hour;
    minute = settings.minute;
    enabled = settings.enabled;
}

struct WiFiSettings {
        char ssid [32];
        char password [32];
        uint8_t mac[6];
        bool enabled;
};

void classPersistent::storeWiFi(const char* pssid, const char *ppassword, const uint8_t* pmac, const bool enabled) {
    struct WiFiSettings settings;
    strcpy(settings.ssid, pssid);
    strcpy(settings.password, ppassword);
    memcpy(settings.mac, pmac, 6);
    settings.enabled = enabled;
    EEPROM.put(WIFI_ADDRESS, settings);
    EEPROM.commit();
}

void classPersistent::fetchWiFi(char* pssid, char *ppassword, uint8_t* pmac, bool &enabled) {
    struct WiFiSettings settings;
    EEPROM.get(WIFI_ADDRESS, settings);
    strcpy(pssid, settings.ssid);
    strcpy(ppassword, settings.password);
    memcpy(pmac, settings.mac, 6);
    enabled = settings.enabled;
}
