#ifndef __PERSISTENT__
#define __PERSISTENT__

class classPersistent {
public:
    classPersistent();

    void storeAlarm(int hour, int minute, bool enabled, int tune);
    void fetchAlarm(int &hour, int &minute, bool &enabled, int &tune);

    void storeWiFi(const char* pssid, const char *ppassword, const uint8_t* pmac, const bool enabled);
    void fetchWiFi(char* pssid, char *ppassword, uint8_t* pmac, bool &enabled);
};

extern classPersistent Persistent;

#endif
