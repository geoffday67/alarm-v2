#ifndef __ALARM_MANAGER__
#define __ALARM_MANAGER__

#include "Timezone.h"

class classAlarmManager {
private:
    int hour, minute;
    bool enabled;
    time_t currentTime, previousTime, wakeTime, snoozeTime, alarmTime;
    int tune;

    void calculateWakeTime();
    void playTune();
    void stopTune();

    void setSnoozeTime(time_t time);
    void setWakeTime(time_t time);
    void setAlarmTime(time_t time);

public:
    classAlarmManager();
    void initialise(time_t time);
    void setAlarm(int hour, int minute);
    void getAlarm(int &hour, int &minute);
    bool isEnabled();
    bool isSnoozing();
    bool isWakeTime();
    bool isSounding();
    void setCurrentTime(time_t time);
    time_t getCurrentTime();
    void enableAlarm();
    void disableAlarm();
    void alarmOn();
    void alarmOff();
    void alarmSnooze(int minutes);
    void setTune(int index);
    int getTune();
};

extern classAlarmManager AlarmManager;

#endif