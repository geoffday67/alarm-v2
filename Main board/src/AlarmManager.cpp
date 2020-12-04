#include "AlarmManager.h"
#include "Persistent.h"
#include "Pins.h"
#include "Timer.h"

classAlarmManager AlarmManager;

classAlarmManager::classAlarmManager() {
    Persistent.fetchAlarm(hour, minute, enabled);
    snoozeTime = 0;
    currentTime = 0;
    pinMode(ALARM_PIN, OUTPUT);
}

bool classAlarmManager::isAlarmSet() {
    return enabled;
}

bool classAlarmManager::isSnoozing() {
    return snoozeTime != 0;
}

void classAlarmManager::setAlarm(int hour, int minute) {
    this->hour = hour;
    this->minute = minute;
    snoozeTime = 0;
    enabled = true;
    Persistent.storeAlarm(hour, minute, enabled);
}

void classAlarmManager::enableAlarm() {
    resetSnooze();
    enabled = true;
    Persistent.storeAlarm(hour, minute, enabled);
}

void classAlarmManager::disableAlarm() {
    enabled = false;
    Persistent.storeAlarm(hour, minute, enabled);
}

// Get the time of the next alarm, or zero if there isn't one
time_t classAlarmManager::getAlarmTime() {
    if (!isAlarmSet()) {
        return 0;
    }

    if (isSnoozing()) {
        return snoozeTime;
    }

    tmElements_t elements;
    breakTime(currentTime, elements);
    elements.Hour = hour;
    elements.Minute = minute;
    elements.Second = 0;
    time_t alarm_time = makeTime(elements);
    if (alarm_time < currentTime) {
        alarm_time += SECS_PER_DAY;
    }

    return alarm_time;
}

void classAlarmManager::resetSnooze() {
    snoozeTime = 0;
}

void classAlarmManager::snoozeAlarm(int minutes) {
    snoozeTime = currentTime + (minutes * SECS_PER_MIN);
    snoozeTime = (snoozeTime / 60) * 60;        // Set seconds to zero
}

void classAlarmManager::setCurrentTime(time_t time) {
    currentTime = time;
}

time_t classAlarmManager::getCurrentTime() {
    return currentTime;
}

void classAlarmManager::alarmOn() {
    digitalWrite(ALARM_PIN, HIGH);
    Serial.println("Alarm board switched on");

    TimerManager.start(new Timer ("alarm-on", 2000, startAlarm));
}

void classAlarmManager::alarmOff() {
    Serial1.write('d');
    Serial.println("Alarm off");

    TimerManager.start(new Timer ("alarm-off", 12000, stopAlarm));
}

void classAlarmManager::startAlarm() {
    Serial1.write('1');
    Serial.println("Alarm on");
}

void classAlarmManager::stopAlarm() {
    digitalWrite(ALARM_PIN, LOW);
    Serial.println("Alarm board switched off");
}