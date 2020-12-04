#include <Arduino.h>
#include <NeoSWSerial.h>
#include <SoftwareSerial.h>

// Pins
#define SOUND_BUSY    2
#define SOUND_TX      3
#define SOUND_RX      4
#define COMMAND_RX    6
#define COMMAND_TX    7
#define MOTOR_1       8
#define MOTOR_2       9
#define MOTOR_SENSE   A0

SoftwareSerial soundSerial(SOUND_RX, SOUND_TX);
SoftwareSerial commandSerial(COMMAND_RX, COMMAND_TX);

bool alarmOn = false;
int alarmTrack, alarmVolume;
bool repeatPending = false;
long repeatStart;
bool movingDown = false;
bool movingUp = false;

void startDown() {
  // Set the motor controller for down
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);

  movingDown = true;
  movingUp = false;

  Serial.println("Moving down");
}

void startUp() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, HIGH);

  movingDown = false;
  movingUp = true;

  Serial.println("Moving up");
}

void stopMotor() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);

  movingDown = false;
  movingUp = false;

  Serial.println("Motor stopped");
}

void resetSound() {
  byte command[] = {0x7E, 0xFF, 0x06, 0x0B, 0x00, 0x00, 0x00, 0xEF};
  soundSerial.write(command, 8);

  Serial.println("Sound reset");
}

void setVolume(int volume) {
  byte command[] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x00, 0xEF};
  command[5] = (volume >> 8) & 0xFF;
  command[6] = volume & 0xFF;
  soundSerial.write(command, 8);

  Serial.print("Volume set to ");
  Serial.println(volume);
}

void playTrack(int track) {
  byte command[] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0xEF};
  command[5] = (track >> 8) & 0xFF;
  command[6] = track & 0xFF;
  soundSerial.write(command, 8);

  Serial.print("Playing track ");
  Serial.println(track);
}

void stopSound() {
  byte command[] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xEF};
  soundSerial.write(command, 8);

  Serial.println("Sound stopped");
}

void startAlarm(int track) {
  alarmOn = true;
  repeatPending = false;
  alarmTrack = track;
  alarmVolume = 20;

  resetSound();
  delay(100);
  setVolume(alarmVolume);
  delay(100);
  playTrack(alarmTrack);

  startUp();
}

void stopAlarm() {
  alarmOn = false;
  repeatPending = false;

  stopSound();
  startDown();
}

void handleSoundFinished() {
  if (alarmOn) {
    repeatStart = millis();
    repeatPending = true;
  }
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting");

  soundSerial.begin(9600);
  Serial.println("Sound serial started");

  commandSerial.begin(9600);
  Serial.println("Command serial started");

  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  movingDown = false;
  movingUp = false;
  Serial.println("Motor initialised");

  attachInterrupt(digitalPinToInterrupt(SOUND_BUSY), handleSoundFinished, RISING);
  Serial.println("End of track handler initialised");
}

void loop() {
  // Check for motor stalled at top or bottom
  if (movingDown) {
    if (analogRead(MOTOR_SENSE) > 50) {
      stopMotor();
    }
  }
  if (movingUp) {
    if (analogRead(MOTOR_SENSE) < 450) {
      stopMotor();
    }
  }

  // Check for repeating the track
  if (repeatPending) {
    if (millis() - repeatStart > 2000) {
      // It's been 2 seconds since a repeat was requested, adjust the volume and play it again.
      if (alarmOn) {
        alarmVolume += 5;
        if (alarmVolume > 30) {
          alarmVolume = 30;
        }
        setVolume(alarmVolume);
        delay(100);
        playTrack(alarmTrack);
      }
      repeatPending = false;
    }
  }

  int n;

  // Check for a command
  int command = commandSerial.read();
  if (command != -1) {
    n = command;
  } else {
    n = Serial.read();
  }

  if (n >= '1' && n <= '9') {
    startAlarm(n - '0');
    delay(1000);
  } else if (n == 'd') {
    stopAlarm();
    delay(1000);
  } else if (n == 's') {
    stopMotor();
  }
}