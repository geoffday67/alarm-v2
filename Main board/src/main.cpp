#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SD.h>
#include "EventManager.h"
#include "Output.h"
#include "MainScreen.h"
#include "RTClib.h"
#include <Timezone.h>
#include "AlarmManager.h"
#include "Pins.h"
#include "NetworkManager.h"
#include "Timer.h"
#include "Light.h"

#define INACTIVITY_MS   10000

RTC_DS3231 rtc;
time_t lastTime = 0;
int keyPressed = -1;
bool sent = false;
int lastHour = 0;
time_t lastKey;
bool active;

TimeChangeRule BST = {"BST", Last, Sun, Mar, 1, 60}; //British Summer Time
TimeChangeRule GMT = {"GMT", Last, Sun, Oct, 2, 0};  //Standard Time
Timezone UK(BST, GMT);

TimeChangeRule CEST = {"CEST", Last, Sun, Mar, 2, 120}; //Central European Summer Time
TimeChangeRule CET = {"CET ", Last, Sun, Oct, 3, 60};   //Central European Standard Time
Timezone CE(CEST, CET);

ICACHE_RAM_ATTR void handleKeypad() {
  int a = digitalRead(16);
  int b = digitalRead(13);
  int c = digitalRead(12);
  int d = digitalRead(14);

  keyPressed = ((b*2)+a)*3 + (d*2) + c + 1;

  if (keyPressed == 11) {
    keyPressed = 0;
  }
}

void setup() {
  // Initialise serial
  Serial.begin(115200);
  Serial.println();
  Serial.println("Serial started");

  // Initialise display
  Output.begin();
  Output.addText(10, 10, "Starting");
  Output.flush();
  Serial.println("Display started");

  // Initialise serial for communication with alarm board
  Serial1.begin(9600);
  Serial.println("Serial 1 started");

  // Initialise RTC
  if (rtc.begin()) {
    Serial.println("RTC started");
  } else {
    Serial.println("Error starting RTC");
  }

  time_t utc = rtc.now().unixtime();
  time_t localTime = UK.toLocal(utc);
  AlarmManager.initialise(localTime);
  lastTime = utc;
  Serial.println("Alarm manager started");

  // Initialise light level monitoring
  lastKey = millis();
  active = true;
  Light.initialise();
  Serial.println("Light level started");

  // Initialise keypad
  pinMode (KEYPAD_AVAILABLE, INPUT);
  attachInterrupt(KEYPAD_AVAILABLE, handleKeypad, RISING);

  MainScreen.activate();
}

void loop() {
  TimerManager.loop();
  Light.loop();
  
  // Check for key inactivity
  if (active && millis() - lastKey > INACTIVITY_MS) {
    active = false;
    EventManager.queueEvent(new InactivityEvent());
  }

  // Check for a key press and send a key event
  if (keyPressed != -1) {
    EventManager.queueEvent(new KeyEvent(keyPressed));
    keyPressed = -1;
    lastKey = millis();
    active = true;
  }

  // If the time in seconds is different from last time then send a time event
  time_t utc = rtc.now().unixtime();
  if (utc != lastTime) {
    time_t localTime = UK.toLocal(utc);
    AlarmManager.setCurrentTime(localTime);
    EventManager.queueEvent(new TimeEvent(localTime));
    lastTime = utc;

    // If it's 3AM now and it wasn't last time then start an NTP session
    DateTime dt = DateTime(localTime);
    if (dt.hour() == 3 && lastHour == 2) {
      NetworkManager.syncTime();
    }
    lastHour = dt.hour();
  }

  EventManager.processEvents();

  //delay(10);
}