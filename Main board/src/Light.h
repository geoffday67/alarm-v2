#ifndef __LIGHT__
#define __LIGHT__

#include "EventManager.h"
#include "EventReceiver.h"

class classLight: public EventReceiver {
public:
    void initialise();
    void loop();
    void setAlarmSounding(bool sounding);
    virtual bool onEvent(Event* pevent);

private:
    bool isDark, keypadActive, alarmSounding, stateChanged;
    void processState();
};

extern classLight Light;

#endif