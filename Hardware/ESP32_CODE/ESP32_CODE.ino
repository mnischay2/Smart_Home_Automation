#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

// WiFi credentials
#define WIFI_SSID "Nischay_iphone"
#define WIFI_PASSWORD "nnnnnnnn"

// Firebase credentials
#define API_KEY "AIzaSyAvjzVHJFzofSYdZGRx2aZBS-AZdpFAif4"
#define DATABASE_URL "https://smartha-5709a-default-rtdb.firebaseio.com/"
#define USER_EMAIL "mnischay2@gmail.com"
#define USER_PASSWORD "ciphering"

// Pin definitions
#define BUTTON1_PIN 14
#define BUTTON2_PIN 27
#define RELAY1_PIN 32
#define RELAY2_PIN 33

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// States and debounce
int button1State = HIGH, lastButton1State = HIGH;
int button2State = HIGH, lastButton2State = HIGH;
unsigned long lastDebounce1 = 0, lastDebounce2 = 0;
const unsigned long debounceDelay = 50;
int relay1State = 0, relay2State = 0;
int prevRelay1 = -1, prevRelay2 = -1;

bool isFirebaseReady = false;
unsigned long lastFirebaseCheck = 0;
const unsigned long firebasePollInterval = 1000; // check every 1s

void setup() {
  Serial.begin(115200);

  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);

  Serial.print("🔌 Connecting to WiFi");
  unsigned long start = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - start < 10000) {
    Serial.print(".");
    delay(500);
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✅ WiFi Connected");

    config.api_key = API_KEY;
    config.database_url = DATABASE_URL;
    auth.user.email = USER_EMAIL;
    auth.user.password = USER_PASSWORD;

    Firebase.begin(&config, &auth);
    Firebase.reconnectWiFi(true);

    Serial.println("🔐 Signing into Firebase...");
    start = millis();
    while (!auth.token.uid.length() && millis() - start < 10000) {
      delay(500);
    }

    if (auth.token.uid.length()) {
      isFirebaseReady = true;
      Serial.println("✅ Firebase Authenticated");

      // Initial fetch of relay states
      Firebase.RTDB.getInt(&fbdo, "/relay1/state") ? relay1State = fbdo.intData() : 0;
      Firebase.RTDB.getInt(&fbdo, "/relay2/state") ? relay2State = fbdo.intData() : 0;
    } else {
      Serial.println("❌ Firebase Auth Failed");
    }
  } else {
    Serial.println("\n❌ WiFi not connected.");
  }

  digitalWrite(RELAY1_PIN, relay1State);
  digitalWrite(RELAY2_PIN, relay2State);
  prevRelay1 = relay1State;
  prevRelay2 = relay2State;
}

void loop() {
  handleButton(BUTTON1_PIN, button1State, lastButton1State, lastDebounce1,
               relay1State, RELAY1_PIN, "/relay1/state", "Relay 1");

  handleButton(BUTTON2_PIN, button2State, lastButton2State, lastDebounce2,
               relay2State, RELAY2_PIN, "/relay2/state", "Relay 2");

  // 🔄 Firebase polling every 1s
  if (isFirebaseReady && millis() - lastFirebaseCheck > firebasePollInterval) {
    lastFirebaseCheck = millis();

    if (Firebase.RTDB.getInt(&fbdo, "/relay1/state")) {
      int val = fbdo.intData();
      if (val != relay1State) {
        relay1State = val;
        digitalWrite(RELAY1_PIN, relay1State);
        Serial.printf("🌐 Relay 1 updated from Firebase: %d\n", relay1State);
      }
    }

    if (Firebase.RTDB.getInt(&fbdo, "/relay2/state")) {
      int val = fbdo.intData();
      if (val != relay2State) {
        relay2State = val;
        digitalWrite(RELAY2_PIN, relay2State);
        Serial.printf("🌐 Relay 2 updated from Firebase: %d\n", relay2State);
      }
    }
  }

  // Reconnect logic
  if (!isFirebaseReady && WiFi.status() == WL_CONNECTED && auth.token.uid.length()) {
    isFirebaseReady = true;
    Serial.println("🔄 Firebase connection restored.");
  }
}

void handleButton(int pin, int &state, int &last, unsigned long &lastDebounce,
                  int &relayState, int relayPin, const char *path, const char *label) {
  int reading = digitalRead(pin);
  if (reading != last) lastDebounce = millis();

  if ((millis() - lastDebounce) > debounceDelay) {
    if (reading != state) {
      state = reading;
      if (state == LOW) {
        relayState = !relayState;
        digitalWrite(relayPin, relayState);
        Serial.printf("🔘 %s toggled: %d\n", label, relayState);

        if (isFirebaseReady) {
          if (Firebase.RTDB.setInt(&fbdo, path, relayState)) {
            Serial.println("✅ Firebase synced");
          } else {
            Serial.printf("❌ Sync failed: %s\n", fbdo.errorReason().c_str());
          }
        }
      }
    }
  }

  last = reading;
}
