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
int relay1State = 0;
int relay2State = 0;
bool isFirebaseReady = false;

void setup() {
  Serial.begin(115200);

  // Setup pins
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  digitalWrite(RELAY1_PIN, relay1State);
  digitalWrite(RELAY2_PIN, relay2State);

  // WiFi auto-reconnect
  WiFi.setAutoReconnect(true);
  WiFi.persistent(true);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("🔌 Connecting to WiFi");

  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 10000) {
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
    unsigned long authStart = millis();
    while (!auth.token.uid.length() && millis() - authStart < 10000) {
      delay(500);
    }

    if (auth.token.uid.length()) {
      Serial.println("✅ Firebase Authenticated");
      isFirebaseReady = true;

      // Get initial relay states from Firebase
      if (Firebase.RTDB.getInt(&fbdo, "/relay1/state"))
        relay1State = fbdo.intData();
      if (Firebase.RTDB.getInt(&fbdo, "/relay2/state"))
        relay2State = fbdo.intData();
    } else {
      Serial.println("❌ Firebase Auth Failed. Running in offline mode.");
    }
  } else {
    Serial.println("\n❌ WiFi not connected. Running in offline mode.");
  }

  digitalWrite(RELAY1_PIN, relay1State);
  digitalWrite(RELAY2_PIN, relay2State);
}

void loop() {
  handleButton(BUTTON1_PIN, button1State, lastButton1State, lastDebounce1,
               relay1State, RELAY1_PIN, "/relay1/state", "Relay 1");

  handleButton(BUTTON2_PIN, button2State, lastButton2State, lastDebounce2,
               relay2State, RELAY2_PIN, "/relay2/state", "Relay 2");

  // Optional: try reconnecting if Firebase was not ready
  if (!isFirebaseReady && WiFi.status() == WL_CONNECTED && auth.token.uid.length()) {
    isFirebaseReady = true;
    Serial.println("🔄 Firebase connection restored.");
  }
}

void handleButton(int pin, int &state, int &last, unsigned long &lastDebounce,
                  int &relayState, int relayPin, const char *path, const char *label) {
  int reading = digitalRead(pin);

  if (reading != last) {
    lastDebounce = millis();
  }

  if ((millis() - lastDebounce) > debounceDelay) {
    if (reading != state) {
      state = reading;
      if (state == LOW) {
        relayState = !relayState;
        digitalWrite(relayPin, relayState);
        Serial.printf("%s toggled to: %d\n", label, relayState);

        if (isFirebaseReady) {
          if (Firebase.RTDB.setInt(&fbdo, path, relayState)) {
            Serial.println("✅ Firebase synced");
          } else {
            Serial.printf("❌ Firebase sync error: %s\n", fbdo.errorReason().c_str());
          }
        }
      }
    }
  }
  last = reading;
}
