#include "NetworkManager.h"
#include "Persistent.h"
#include "RTClib.h"
#include <ArduinoHttpClient.h>

#define MQTT_SERVER     "geoffs-mac-mini.local"
#define MQTT_PORT       1883
#define MQTT_CLIENT     "alarmv2"
#define KETTLE_TOPIC    "alarm/kettle"
#define NTP_TOPIC       "alarm/ntp"

#define KETTLE_HOST     "192.168.68.106"
#define KETTLE_PORT     1968
#define KETTLE_URI      "/interface"

#define NTP_SERVER        "time.google.com"
#define NTP_PORT          123
#define NTP_OFFSET        2208988800UL

classNetworkManager NetworkManager;

extern RTC_DS3231 rtc;

classNetworkManager::classNetworkManager() {
  disconnectWiFi();
  Persistent.fetchWiFi(ssid, password, enabled);

  mqtt.setClient(wifi);
  mqtt.setServer(MQTT_SERVER, MQTT_PORT);
}

void classNetworkManager::syncTime() {
  byte ntpBuffer[48];
  IPAddress address;
  uint32_t ntpTime;
  uint32_t unixTime;
  unsigned long start;
  char s [64];
  DateTime dt;

  if (!connectWiFi()) {
    Serial.println("Error connecting WiFi");
    goto Exit;
  }

  ntpUDP.begin(NTP_PORT);

  if (!WiFi.hostByName(NTP_SERVER, address)) {
    Serial.println("Error looking up time server address");
    goto Exit;
  }
  Serial.print ("Time server address: ");
  Serial.println(address);

  memset(ntpBuffer, 0, 48);
  ntpBuffer[0] = 0b11100011;
  ntpUDP.beginPacket(address, NTP_PORT);
  ntpUDP.write(ntpBuffer, 48);
  ntpUDP.endPacket();

  Serial.println("Waiting for NTP response");
  start = millis();
  while (ntpUDP.parsePacket() == 0) {
      if (millis() - start > 10000) {
        Serial.println("Timeout");
        goto Exit;
      }
      delay(10);
  }

  ntpUDP.read(ntpBuffer, 48);
  ntpTime = (ntpBuffer[40] << 24) | (ntpBuffer[41] << 16) | (ntpBuffer[42] << 8) | ntpBuffer[43];
  unixTime = ntpTime - NTP_OFFSET;

  dt = DateTime(unixTime);
  rtc.adjust(dt);
  sprintf (s, "RTC adjusted to %s UTC\n", dt.timestamp(DateTime::TIMESTAMP_FULL).c_str());
  Serial.printf(s);

  if (mqtt.connect(MQTT_CLIENT)) {
    mqtt.publish(NTP_TOPIC, s, true);
    mqtt.disconnect();
  }

Exit:
  disconnectWiFi();
}

void classNetworkManager::enable() {
  enabled = true;
  Persistent.storeWiFi(ssid, password, enabled);
}

void classNetworkManager::disable() {
  enabled = false;
  Persistent.storeWiFi(ssid, password, enabled);
}

void classNetworkManager::setSSID(const char* pssid, const char *ppassword) {
  strcpy (ssid, pssid);
  strcpy (password, ppassword);
  enabled = true;
  Persistent.storeWiFi(ssid, password, enabled);
}

void classNetworkManager::getSSID(char *pssid) {
  strcpy (pssid, ssid);
}

bool classNetworkManager::getEnabled() {
  return enabled;
}

void classNetworkManager::kettleOn() {
  if (!connectWiFi()) {
    Serial.println("Error connecting WiFi");
    return;
  }

  HttpClient client = HttpClient(wifi, KETTLE_HOST, KETTLE_PORT);
  char body[] = "operation=433&channel=3&command=on";
  int status;

  Serial.printf ("Sending: %s\n", body);

  client.beginRequest();
  client.post(KETTLE_URI);
  client.sendHeader("Content-Type", "application/x-www-form-urlencoded");
  client.sendHeader("Content-Length", strlen(body));
  client.sendBasicAuth("wario", "mansion1");
  client.beginBody();
  client.print(body);
  client.endRequest();

  Serial.printf ("Received code %d: %s\n", client.responseStatusCode(), client.responseBody().c_str());

Exit:
  disconnectWiFi();
}

bool classNetworkManager::connectWiFi() {
  // TODO Use reference counting in case wifi is being used elsewhere too.

  bool result = false;
  unsigned long start;

  if (!enabled) {
    Serial.println("WiFi disabled");
    goto exit;
  }

  WiFi.hostname("Moomin alarm");
  WiFi.begin(ssid, password);
  Serial.printf("Connecting to %s ", ssid);

  start = millis();
  while (WiFi.status() != WL_CONNECTED) {
      if (millis() - start > 20000) {
          Serial.println();
          Serial.println("Timed out connecting to access point");
          goto exit;
      }
      delay(100);
      Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected, IP address: ");
  Serial.println(WiFi.localIP());

  result = true;

exit:
  return result;
}

void classNetworkManager::disconnectWiFi() {
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();  
  Serial.println("WiFi disconnected");
}
