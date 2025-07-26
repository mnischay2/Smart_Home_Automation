#include <WiFi.h>
#include <Firebase_ESP_Client.h>

// WiFi credentials
#define WIFI_SSID "Nischay_iphone"
#define WIFI_PASSWORD "nnnnnnnn"

// Firebase credentials
#define API_KEY "AIzaSyAvjzVHJFzofSYdZGRx2aZBS-AZdpFAif4"
#define DATABASE_URL "https://smartha-5709a-default-rtdb.firebaseio.com/"
#define USER_EMAIL "mnischay2@gmail.com"
#define USER_PASSWORD "ciphering"

// Button pins
#define BUTTON1_PIN 14
#define BUTTON2_PIN 27

// Relay control pins (MOC3021)
#define RELAY1_PIN 32
#define RELAY2_PIN 33

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

// States
int button1State = HIGH, lastButton1State = HIGH, relay1Value = 0;
int button2State = HIGH, lastButton2State = HIGH, relay2Value = 0;
unsigned long lastDebounce1 = 0, lastDebounce2 = 0;
const unsigned long debounceDelay = 50;

unsigned long lastCheck = 0;
const unsigned long checkInterval = 1000;  // sync every 1 second

void setup() {
  Serial.begin(115200);
  pinMode(BUTTON1_PIN, INPUT_PULLUP);
  pinMode(BUTTON2_PIN, INPUT_PULLUP);
  pinMode(RELAY1_PIN, OUTPUT);
  pinMode(RELAY2_PIN, OUTPUT);

  // Connect to WiFi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n✅ Connected to WiFi");

  // Setup Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // Wait for authentication
  Serial.println("🔐 Signing in...");
  unsigned long startTime = millis();
  while (!auth.token.uid.length()) {
    if (millis() - startTime > 10000) {
      Serial.println("❌ Auth timeout.");
      return;
    }
    delay(500);
  }

  Serial.println("✅ Authenticated");

  // Initial sync from Firebase
  Firebase.RTDB.getInt(&fbdo, "/relay1/state");
  relay1Value = fbdo.intData();
  digitalWrite(RELAY1_PIN, relay1Value);

  Firebase.RTDB.getInt(&fbdo, "/relay2/state");
  relay2Value = fbdo.intData();
  digitalWrite(RELAY2_PIN, relay2Value);
}

void loop() {
  handleButton(BUTTON1_PIN, button1State, lastButton1State, relay1Value, lastDebounce1, "/relay1/state", "Relay 1", RELAY1_PIN);
  handleButton(BUTTON2_PIN, button2State, lastButton2State, relay2Value, lastDebounce2, "/relay2/state", "Relay 2", RELAY2_PIN);

  // Check for remote Firebase changes every second
  if (millis() - lastCheck > checkInterval) {
    lastCheck = millis();
    syncWithFirebase();
  }
}

void handleButton(int pin, int &state, int &last, int &value, unsigned long &lastDebounce, const char* path, const char* label, int relayPin) {
  int reading = digitalRead(pin);
  if (reading != last) {
    lastDebounce = millis();
  }

  if ((millis() - lastDebounce) > debounceDelay) {
    if (reading != state) {
      state = reading;

      if (state == LOW) {
        value = !value;
        Serial.print(label);
        Serial.print(" toggled: ");
        Serial.println(value);

        digitalWrite(relayPin, value);

        if (Firebase.RTDB.setInt(&fbdo, path, value)) {
          Serial.println("✅ Firebase updated");
        } else {
          Serial.print("❌ Firebase error: ");
          Serial.println(fbdo.errorReason());
        }
      }
    }
  }

  last = reading;
}

void syncWithFirebase() {
  if (Firebase.RTDB.getInt(&fbdo, "/relay1/state")) {
    int newVal = fbdo.intData();
    if (newVal != relay1Value) {
      relay1Value = newVal;
      digitalWrite(RELAY1_PIN, relay1Value);
      Serial.println("🔁 Synced Relay 1 from Firebase");
    }
  }

  if (Firebase.RTDB.getInt(&fbdo, "/relay2/state")) {
    int newVal = fbdo.intData();
    if (newVal != relay2Value) {
      relay2Value = newVal;
      digitalWrite(RELAY2_PIN, relay2Value);
      Serial.println("🔁 Synced Relay 2 from Firebase");
    }
  }
}
