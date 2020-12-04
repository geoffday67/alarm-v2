#ifndef __ALARM_MANAGER__
#define __ALARM_MANAGER__

#include "Timezone.h"

class classAlarmManager {
private:
    int hour;
    int minute;
    bool enabled;
    time_t snoozeTime;
    time_t currentTime;

    static void startAlarm();
    static void stopAlarm();

public:
    classAlarmManager();
    void setAlarm(int hour, int minute);
    bool isAlarmSet();
    bool isSnoozing();
    time_t getAlarmTime();
    void setCurrentTime(time_t time);
    time_t getCurrentTime();
    void enableAlarm();
    void disableAlarm();
    void snoozeAlarm(int minutes);
    void resetSnooze();
    void alarmOn();
    void alarmOff();
};

extern classAlarmManager AlarmManager;

#endif