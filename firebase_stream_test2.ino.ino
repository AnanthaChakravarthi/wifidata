#include<ESP8266WiFi.h>
#include <FirebaseArduino.h>
FirebaseArduino FirebaseStream;
#define FIREBASE_AUTH "9eQbDkxJ6wbPAZpi5us5S3XnsgbLluwMywA5HvEo"
String FIREBASE_HOST="mobile-control-home.firebaseio.com";
const char* WIFI_SSID="chakri";
const char* WIFI_PASSWORD="asdfghjkl";
void setup() {
  Serial.begin(115200);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);  
  FirebaseStream.begin(FIREBASE_HOST, FIREBASE_AUTH);
  FirebaseStream.stream("/users");
}

int n = 0;
void loop() {
  Serial.println("loop");
  if (FirebaseStream.available()) {
    FirebaseObject event = FirebaseStream.readEvent();
    Serial.print("event: ");
    Serial.println(event.getString("event"));
    Serial.print("path: ");
    Serial.println(event.getString("path"));
    Serial.print("data: ");
    Serial.println(event.getInt("data"));
  }
  Serial.println(ESP.getFreeHeap());
  delay(1000);
}
