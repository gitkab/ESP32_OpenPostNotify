#include <WiFi.h>
#include <HTTPClient.h>
#include <esp_deep_sleep.h>

// WiFi Setup
const char* STA_ID = "(Define Wi-Fi SSID)";
const char* STA_PW = "(Define Wi-Fi Password)";

// Webhooks Address
const char* IFTTT_WH = "http://maker.ifttt.com/trigger/[EVENT]/with/key/[KEY]";

void setup() {
  Serial.begin(115200);
  Serial.print("WiFi Connecting");

  WiFi.mode(WIFI_STA);
  WiFi.begin(STA_ID, STA_PW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(200);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi Connected");

  HTTPClient http;
  http.begin(IFTTT_WH); /* HTTP URL Define */

  Serial.print("[HTTP] begin...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();

  // httpCode will be negative on error
  if (httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... Result: %d\n", httpCode);
    // file found at server
    if (httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.println(payload);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }

  http.end();
  WiFi.disconnect();

  Serial.println("Terminated. Please Reset.");
  Serial.println("Deep Sleeping...");

  delay(100);
  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH, ESP_PD_OPTION_OFF);
  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);
  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);
  esp_deep_sleep_pd_config(ESP_PD_DOMAIN_MAX, ESP_PD_OPTION_OFF);
  esp_deep_sleep_start();
}

void loop() {
  // Do nothing.
}