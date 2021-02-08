#include "AlarmManager.h"
#include "Persistent.h"
#include "Pins.h"
#include "Timer.h"
#include "EventManager.h"

/*
Wake time - time at which regular alarm is due.
Enabled - whether any alarm should sound (keep track of time, just don't sound).
Snooze time - time at which alarm should go off when snoozing. Set n minutes after
    alarm last went off. 0 => not snoozing.
Alarm time - time alarm last went off. Used to calculate snooze time.
*/

classAlarmManager AlarmManager;

char *printableTime(time_t t) {
    static char s[64];
    tmElements_t elements;
    breakTime(t, elements);
    sprintf(s, "%02d:%02d:%02d %d-%d-%d", elements.Hour, elements.Minute, elements.Second,
        elements.Day, elements.Month, elements.Year + 1970);
    return s;
}

void classAlarmManager::setWakeTime(time_t time) {
    wakeTime = time;
    Serial.printf ("Wake time set to %s\n", printableTime(time));
}

void classAlarmManager::setSnoozeTime(time_t time) {
    snoozeTime = time;
    Serial.printf ("Snooze time set to %s\n", time == 0 ? "zero" : printableTime(time));
}

void classAlarmManager::setAlarmTime(time_t time) {
    alarmTime = time;
    Serial.printf ("Alarm time set to %s\n", printableTime(time));
}

classAlarmManager::classAlarmManager() {
    Persistent.fetchAlarm(hour, minute, enabled, tune);
}

void classAlarmManager::calculateWakeTime() {
    tmElements_t elements;
    breakTime(currentTime, elements);
    elements.Hour = hour;
    elements.Minute = minute;
    elements.Second = 0;
    time_t target = makeTime(elements);

    if (target < currentTime) {
        target += SECS_PER_DAY;
    }

    setWakeTime(target);
}

void classAlarmManager::initialise(time_t time) {
    currentTime = time;
    previousTime = time;
    calculateWakeTime();
}

void classAlarmManager::setAlarm(int hour, int minute) {
    this->hour = hour;
    this->minute = minute;
    calculateWakeTime();
    enableAlarm();
}

void classAlarmManager::getAlarm(int &hour, int &minute) {
    hour = this->hour;
    minute = this->minute;
}

void classAlarmManager::enableAlarm() {
    enabled = true;
    Persistent.storeAlarm(hour, minute, enabled, tune);

    setSnoozeTime(0);
}

void classAlarmManager::disableAlarm() {
    enabled = false;
    Persistent.storeAlarm(hour, minute, enabled, tune);

    setSnoozeTime(0);
}

bool classAlarmManager::isEnabled() {
    return enabled;
}

void classAlarmManager::setCurrentTime(time_t time) {
    currentTime = time;

    if (isWakeTime()) {
        setAlarmTime(currentTime);
        EventManager.emptyQueue();
        EventManager.queueEvent(new AlarmEvent(currentTime));
    }

    if (currentTime > wakeTime) {
        setWakeTime (wakeTime + SECS_PER_DAY);
    }
}

time_t classAlarmManager::getCurrentTime() {
    return currentTime;
}

void classAlarmManager::alarmOn() {
    playTune();
}

void classAlarmManager::alarmOff() {
    stopTune();
    setSnoozeTime(0);
}

void classAlarmManager::alarmSnooze(int minutes) {
    stopTune();
    setSnoozeTime (alarmTime + (minutes * SECS_PER_MIN));
}

bool classAlarmManager::isSnoozing() {
    return snoozeTime != 0;
}

bool classAlarmManager::isWakeTime() {
    if (!enabled) {
        return false;
    }

    /*Serial.printf("Current time = %s, ", printableTime(currentTime));
    Serial.printf("Previous time = %s, ", printableTime(previousTime));
    Serial.printf("Wake time = %s\n", printableTime(wakeTime));*/

    bool result;
    time_t target = snoozeTime == 0 ? wakeTime : snoozeTime;

    if (currentTime >= target && previousTime < target) {
        result = true;
    } else {
        result = false;
    }
    previousTime = currentTime;
    return result;
}

void classAlarmManager::setTune(int index) {
    this->tune = index;
    Persistent.storeAlarm(hour, minute, enabled, tune);
}

int classAlarmManager::getTune() {
    return tune;
}

void classAlarmManager::playTune() {
    Serial1.write((char) (0x30 + tune));
    Serial.printf("Playing tune %d\n", tune);
}

void classAlarmManager::stopTune() {
    Serial1.write('d');
    Serial.println("Stopping tune");
}
