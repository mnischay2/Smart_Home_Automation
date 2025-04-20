#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include <addons/TokenHelper.h>
#include <addons/RTDBHelper.h>

/* 1. Define the WiFi credentials */
#define WIFI_SSID "Nischay_iphone"
#define WIFI_PASSWORD "nnnnnnnn"

/* 2. Define the API Key */
#define API_KEY "AIzaSyAvjzVHJFzofSYdZGRx2aZBS-AZdpFAif4"

/* 3. Define the RTDB URL */
#define DATABASE_URL "https://smartha-5709a-default-rtdb.firebaseio.com/" 

/* 4. Define the user Email and password */
#define USER_EMAIL "mnischay2@gmail.com"
#define USER_PASSWORD "ciphering"

// Firebase objects
FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

// Relay GPIO pins
int S1 = 32;
int S2 = 33;
int S3 = 25;
int S4 = 26;

void setup()
{
  Serial.begin(115200);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(S4, OUTPUT);

  digitalWrite(S1,1);
  digitalWrite(S2,1);
  digitalWrite(S3,1);
  digitalWrite(S4,1);
  // Connect to Wi-Fi
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Firebase configuration
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  config.token_status_callback = tokenStatusCallback;
  Firebase.reconnectNetwork(true);
  fbdo.setBSSLBufferSize(4096, 1024);
  fbdo.setResponseSize(2048);
  Firebase.begin(&config, &auth);
  config.timeout.serverResponse = 10 * 1000;
}

void loop()
{
  if (Firebase.ready()){
    int r1v = 0, r2v = 0;

    // Read /relay1/state 
    if (Firebase.RTDB.getInt(&fbdo, F("/relay1/state"), &r1v)) {
      Serial.print("Switch1 status: ");
      Serial.println(r1v);
      digitalWrite(S1, r1v ? HIGH : LOW);
    } else {
      Serial.println("Failed to read /relay1/state");
      Serial.println(fbdo.errorReason());
    }

    // Read /relay2/state
    if (Firebase.RTDB.getInt(&fbdo, F("/relay2/state"), &r2v)) {
      Serial.print("Switch2 status: ");
      Serial.println(r2v);
      digitalWrite(S2, r2v ? HIGH : LOW);
    } else {
      Serial.println("Failed to read /relay2/state");
      Serial.println(fbdo.errorReason());
    }
    int r3v = 0, r4v = 0;
    if (Firebase.RTDB.getInt(&fbdo, F("/relay3/state"), &r3v)) {
      digitalWrite(S3, r3v ? HIGH : LOW);
    }
    if (Firebase.RTDB.getInt(&fbdo, F("/relay4/state"), &r4v)) {
      digitalWrite(S4, r4v ? HIGH : LOW);
    }
    

    delay(2000);
  }
}
