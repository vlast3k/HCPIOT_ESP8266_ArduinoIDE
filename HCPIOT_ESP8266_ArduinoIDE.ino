#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char * headerkeys[] = {"Location"};
int value = 0;
char url[200];

// ---------- CONFIGURATION -----------
const char* ssid     = "ssid";
const char* password = "pass";

char host[] = "iotmmsi024148trial.hanatrial.ondemand.com";
char deviceId[] = "c5c73d69-6a19-4c7d-9da3-b32198ba71f9";
char messageId[] = "2023a0e66f76d20f47d7";
char variableName[] = "co2";
char token[] = "be4e6b1381f6989b195a402420399a8";
// ---------- CONFIGURATION -----------

void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  Serial.printf("\nConnecting to %s\n", ssid);  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.printf("\nWiFi connected\nIP address: %s\n\n", WiFi.localIP().toString().c_str());
}

void loop() {
  ++value;

  sprintf(url, "https://%s/com.sap.iotservices.mms/v1/api/http/data/%s/%s/sync?%s=%d",
                host, deviceId, messageId, variableName, value);

  Serial.printf("connecting to %s\n", url);
  
  HTTPClient http;
  http.begin(url);
  http.addHeader("Content-Type",  "application/json;charset=UTF-8");
  http.addHeader("Authorization", String("Bearer ") + token); 
  http.collectHeaders(headerkeys, 1); // collect Location header, to check redirect location in case of 30x response
  int rc = http.POST("");

  if (rc == 301 || rc == 302) {
    Serial.printf("Redirected to: %s", http.header("Location").c_str());
  } else if (rc > 0) {
    Serial.printf("Payload: [%s]\n", http.getString().c_str());
  } else {
    Serial.println("Failed");
  } 
  
  Serial.println("Closing connection and waiting 15 seconds");
  delay(15000);
}

