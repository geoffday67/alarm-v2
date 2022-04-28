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
        storeAlarm(0, 0, false, 1);

        storeWiFi("", "", false);
    }
}

struct AlarmSettings {
    int hour;
    int minute;
    bool enabled;
    int tune;
};

void classPersistent::storeAlarm(int hour, int minute, bool enabled, int tune) {
    struct AlarmSettings settings;
    settings.hour = hour;
    settings.minute = minute;
    settings.enabled = enabled;
    settings.tune = tune;
    EEPROM.put(ALARM_ADDRESS, settings);
    EEPROM.commit();
}

void classPersistent::fetchAlarm(int &hour, int &minute, bool &enabled, int &tune) {
    struct AlarmSettings settings;
    EEPROM.get(ALARM_ADDRESS, settings);
    hour = settings.hour;
    minute = settings.minute;
    enabled = settings.enabled;
    tune = settings.tune;
}

struct WiFiSettings {
        char ssid [32];
        char password [32];
        bool enabled;
};

void classPersistent::storeWiFi(const char* pssid, const char *ppassword, const bool enabled) {
    struct WiFiSettings settings;
    strcpy(settings.ssid, pssid);
    strcpy(settings.password, ppassword);
    settings.enabled = enabled;
    EEPROM.put(WIFI_ADDRESS, settings);
    EEPROM.commit();
}

void classPersistent::fetchWiFi(char* pssid, char *ppassword, bool &enabled) {
    struct WiFiSettings settings;
    EEPROM.get(WIFI_ADDRESS, settings);
    strcpy(pssid, settings.ssid);
    strcpy(ppassword, settings.password);
    enabled = settings.enabled;
}
