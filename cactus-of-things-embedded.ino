#include "esp_wpa2.h"
#include <WiFi.h>
#include <HTTPClient.h>

#define EAP_IDENTITY "identity"
#define EAP_PASSWORD "password"

#define ENDPOINT "endpoint"

#define MENOPC "Cactus Of Things ESP32 Board"

const char* ssid = "ssid";

WiFiClient client;

const int LIGHT_PIN = 35;
const int MOISTURE_PIN = 34;

// declare reset function
void(* resetFunc) (void) = 0;

/*
 * Connects to wifi
 */
void connectWifi(bool debug) {
    // open serial with 115200 baud rate
    if (debug) {
      Serial.begin(115200);
      delay(10);
      Serial.println(); 
      // print desired ssid
      Serial.println(ssid);
    }

    // configure wifi
    WiFi.disconnect(true);
    
    WiFi.mode(WIFI_STA);
    
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);

    if (debug) {
      // print mac address
      Serial.println("MAC address: ");
      Serial.println(WiFi.macAddress()); 
    }

    // connect
    WiFi.begin(ssid);
    
    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts++ < 10) {
        delay(500);
        if (debug) {
          Serial.print("."); 
        }
    }

    // reset if not connected
    if (WiFi.status() != WL_CONNECTED) {
        resetFunc();
    }

    if (debug) {
      // print current ip address
      Serial.println();
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
    }
}

void setup() {
  connectWifi(false);
  pinMode(LIGHT_PIN, INPUT);
}

/*
 * Sleeps for one minute 
 */
void waitAMinute() {
    delay(60 * 1000);
}

/*
 * Sends the provided reading to the endpoint
 */
void sendReading(int moisture, int light, bool debug) {
  if (debug) {
    Serial.print("Moisture: ");
    Serial.print(moisture);
    Serial.print(", Light: ");
    Serial.println(light);    
  }

  String endpoint = ENDPOINT;

  String path = endpoint + "/readings/store";

  HTTPClient http;

  if (debug) {
    Serial.println("Connecting to " + path); 
  }

  http.begin(path.c_str());

  http.addHeader("Content-Type", "application/json");

  String moistureString = String(moisture);
  String lightString = String(light);

  String requestBody = "[{\"Moisture\":" + moistureString + ", \"Light\":" + lightString + "}]";

  int response = http.POST(requestBody);

  if (debug) {
    Serial.print("Request body: ");
    Serial.println(requestBody);
    Serial.print("Send request, response: ");
    Serial.println(response); 
  }

  http.end();
}

/*
 * Reads the moisture level
 */
int readMoisture() {
  return analogRead(MOISTURE_PIN);
}

/*
 * Reads the light level
 */
int readLight() {
  return 1 - digitalRead(LIGHT_PIN);
}

void loop() {
  waitAMinute();
  sendReading(readMoisture(), readLight(), false);
}
