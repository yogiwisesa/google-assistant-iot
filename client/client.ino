#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

// Relay Pin
int pins[8] = {D2, D3, D4, D5, D6, D7, D8, D9};

const char *ssid = "Yogi AP";
const char *password = "@Merpati07";

String API_ENDPOINT = "https://iot.gogiw.com/relay-project/status";

void setup()
{
  Serial.begin(9600);
  delay(10);

  // Prepare pin
  int i = 0;
  for (i; i < 8; i++) {
    pinMode(pins[i], OUTPUT);
  }

  Serial.print("Connecting Wifi: ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop()
{
  Serial.println("Fetching...");
  if (WiFi.status() == WL_CONNECTED)
  {

    HTTPClient http;
    http.setReuse(true);

    // Second argument is the fingerprint of the website
    // Source: https://forum.arduino.cc/index.php?topic=529371.0
    // find the fingerprint using this website: https://www.grc.com/fingerprints.htm
    http.begin(API_ENDPOINT, "DA:C9:73:A0:F0:CE:EB:EF:C6:6A:DD:30:FE:D0:33:C4:F8:4F:94:D9");
    http.addHeader("Content-Type", "application/json");
    int httpCode = http.GET();

    if (httpCode > 0)
    {
      Serial.println("Success: " + http.getString());

      const size_t capacity = JSON_ARRAY_SIZE(8) + JSON_OBJECT_SIZE(2) + 70;
      DynamicJsonBuffer jsonBuffer(capacity);

      JsonObject &root = jsonBuffer.parseObject(http.getString());
      // JSON to Object using JSON assistant
      // https://arduinojson.org/v5/assistant/
      const char *status = root["status"]; // "ok"

      JsonArray &data = root["data"];

      // Update pin state using API response
      int i = 0;
      for (i; i < 8; i++) {
        digitalWrite(pins[i], boolToWriteValue(data[i]));
      }

      Serial.print("Status: ");
      Serial.println(status);
    }
    else
    {
      Serial.println("FAILED: ");
      Serial.println(http.errorToString(httpCode).c_str());
    }
    http.end(); //Close connection
  }
  // Delay
  delay(10000);
}

uint8_t boolToWriteValue(bool value)
{
  if (value == true)
  {
    return LOW;
  }
  else
  {
    return HIGH;
  }
}
