#include <Arduino.h>
#include <SoftwareSerial.h>
#include <avr/power.h>
#include <avr/sleep.h>

// Pins
#define SOUND_BUSY    2
#define SOUND_TX      3
#define SOUND_RX      4
#define COMMAND_RX    6
#define COMMAND_TX    7
#define MOTOR_1       8
#define MOTOR_2       9
#define LIMIT_TOP     10
#define LIMIT_BOTTOM  11
#define MOTOR_ACTIVE  12

#define SOUND_STARTING    1
#define SOUND_PLAYING     2
#define SOUND_PENDING     3
#define SOUND_OFF         4
#define SOUND_IDLE        5

SoftwareSerial soundSerial(SOUND_RX, SOUND_TX);
SoftwareSerial commandSerial(COMMAND_RX, COMMAND_TX);

bool alarmOn = false;
int alarmTrack, alarmVolume;
long trackFinishedTime;
bool movingDown = false;
bool movingUp = false;
int soundState;

/*
States - waiting for ready, playing, waiting for next play.
*/

void soundSetState (int state) {
  soundState = state;

  Serial.print ("Sound state now ");
  switch (soundState) {
    case SOUND_STARTING: Serial.println("STARTING"); break;
    case SOUND_PLAYING: Serial.println("PLAYING"); break;
    case SOUND_PENDING: Serial.println("PENDING"); break;
    case SOUND_OFF: Serial.println("OFF"); break;
    case SOUND_IDLE: Serial.println("IDLE"); break;
  }
}

void soundPowerOn() {
  //digitalWrite (SOUND_PWR, HIGH);
  soundSetState(SOUND_STARTING);
}

void soundPowerDown() {
  //digitalWrite (SOUND_PWR, LOW);
  soundSetState(SOUND_OFF);
}

void sleep() {
  //soundPowerDown();

  /*power_all_disable();
  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  sleep_mode();*/
  while(1) ;
}

void stopMotor() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_ACTIVE, LOW);

  movingDown = false;
  movingUp = false;

  Serial.println("Motor stopped");
}

void startDown() {
  digitalWrite(MOTOR_1, HIGH);
  digitalWrite(MOTOR_2, LOW);
  digitalWrite(MOTOR_ACTIVE, HIGH);

  movingDown = true;
  movingUp = false;

  Serial.println("Moving down");
}

void startUp() {
  digitalWrite(MOTOR_1, LOW);
  digitalWrite(MOTOR_2, HIGH);
  digitalWrite(MOTOR_ACTIVE, HIGH);

  movingDown = false;
  movingUp = true;

  Serial.println("Moving up");
}

void resetSound() {
  byte command[] = {0x7E, 0xFF, 0x06, 0x0B, 0x00, 0x00, 0x00, 0xEF};
  soundSerial.write(command, 8);

  Serial.println("Sound reset");
}

void setVolume() {
  byte command[] = {0x7E, 0xFF, 0x06, 0x06, 0x00, 0x00, 0x00, 0xEF};
  command[5] = (alarmVolume >> 8) & 0xFF;
  command[6] = alarmVolume & 0xFF;
  soundSerial.write(command, 8);

  Serial.print("Volume set to ");
  Serial.println(alarmVolume);
}

void playTrack() {
  resetSound();
  delay(100);
  setVolume();
  delay(100);

  byte command[] = {0x7E, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0xEF};
  command[5] = (alarmTrack >> 8) & 0xFF;
  command[6] = alarmTrack & 0xFF;
  soundSerial.write(command, 8);

  Serial.print("Playing track ");
  Serial.println(alarmTrack);
}

void stopSound() {
  byte command[] = {0x7E, 0xFF, 0x06, 0x0E, 0x00, 0x00, 0x00, 0xEF};
  soundSerial.write(command, 8);

  Serial.println("Sound stopped");
}

void startAlarm(int track) {
  alarmOn = true;
  alarmTrack = track;
  alarmVolume = 20;

  // If the player is starting up if not it'll start the track when start up is done.
  if (soundState == SOUND_IDLE) {
    playTrack();
    soundSetState(SOUND_PLAYING);
  }

  startUp();
}

void stopAlarmNoDown() {
  alarmOn = false;
  soundSetState(SOUND_IDLE);
  stopSound();
}

void stopAlarm() {
  stopAlarmNoDown();
  startDown();
}

void handleTrackFinished() {
  if (soundState == SOUND_PLAYING) {
    trackFinishedTime = millis();
    soundSetState(SOUND_PENDING);
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

  //pinMode(SOUND_PWR, OUTPUT);
  alarmTrack = -1;
  soundPowerOn();
  Serial.println("Sound initialised");

  pinMode(LIMIT_BOTTOM, INPUT_PULLUP);
  pinMode(LIMIT_TOP, INPUT_PULLUP);
  Serial.println("Limits initialised");
}

void loop() {
  if (movingUp) {
    if (digitalRead(LIMIT_TOP) == LOW) {
      Serial.println("Top reached");
      stopMotor();
    }
  }

  if (movingDown) {
    if (digitalRead(LIMIT_BOTTOM) == LOW) {
      Serial.println("Bottom reached");
      stopMotor();
    }
  }

  // Process sound state
  switch (soundState) {
    case SOUND_STARTING:
        // Check for BUSY low until ready, then start playing alarmTrack, if any.
        // Otherwise just set SOUND_IDLE (powered up but nothing to do).

        // if (not ready) break;
        // Or maybe use the interrupt. Might need to pull the input low.

        // From now on BUSY going high indicates that a track has finished playing.
        attachInterrupt(digitalPinToInterrupt(SOUND_BUSY), handleTrackFinished, RISING);
        Serial.println("End of track handler initialised");

        if (alarmTrack != -1) {
          playTrack();
          soundSetState(SOUND_PLAYING);
        } else {
          soundSetState(SOUND_IDLE);
        }

        break;

      case SOUND_PENDING:
        // Track has finished, we're waiting to play it again.
        if (millis() - trackFinishedTime < 2000 || !alarmOn) {
          break;
        }
        alarmVolume += 5;
        if (alarmVolume > 30) {
          alarmVolume = 30;
        }
        playTrack();
        soundSetState(SOUND_PLAYING);
        break;
  }

  // Check for a command from the command serial first, and if not then from normal serial.
  int command, n;
  n = commandSerial.read();
  if (n != -1) {
    command = n;
  } else {
    command = Serial.read();
  }

  if (command >= '1' && command <= '9') {
    startAlarm(command - '0');
  } else if (command == 'u') {
    startUp();
  } else if (command == 'd') {
    stopAlarm();
  } else if (command == 'm') {
    stopAlarmNoDown();
  } else if (command == 's') {
    stopMotor();
  }
}