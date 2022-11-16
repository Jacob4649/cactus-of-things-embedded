#include "esp_wpa2.h"
#include <WiFi.h>

#define EAP_IDENTITY "identity"
#define EAP_PASSWORD "password"

#define ENDPOINT "endpoint"

#define MENOPC "Cactus Of Things ESP32 Board"

String line;
const char* ssid = "ssid";
WiFiClient client;

// declare reset function
void(* resetFunc) (void) = 0;

void setup() {

    // open serial with 115200 baud rate
    Serial.begin(115200);
    delay(10);
    Serial.println();

    // print desired ssid
    Serial.println(ssid);

    // configure wifi
    WiFi.disconnect(true);
    
    WiFi.mode(WIFI_STA);
    
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY));
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD));
    
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT();
    esp_wifi_sta_wpa2_ent_enable(&config);

    // print mac address
    Serial.println("MAC address: ");
    Serial.println(WiFi.macAddress());

    // connect
    WiFi.begin(ssid);
    
    int attempts = 0;

    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
        delay(500);
        Serial.print(".");
    }

    // reset if not connected
    if (WiFi.status() != WL_CONNECTED) {
        resetFunc();
    }

    // print current ip address
    Serial.println();
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // setup watchdog?
}

void loop() {

}
