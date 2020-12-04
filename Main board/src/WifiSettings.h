#ifndef __WIFI_SETTINGS__
#define __WIFI_SETTINGS__

#include "Screen.h"
#include "EventManager.h"
#include "EventReceiver.h"

class classWifiSettings: public Screen, EventReceiver {
public:
    virtual void activate();
    virtual void deactivate();
    virtual void onEvent(Event* pevent);

private:
    void handleKeyEvent(KeyEvent *pevent);
};

extern classWifiSettings WifiSettings;

#endif