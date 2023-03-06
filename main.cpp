#include <Arduino.h>
#include <Wifi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>
#include "env.h" 

#define pin1_led 18
#define pin2_led 19
#define pin3_led 21

bool pattern [8][3]={  {false,false,false},
                        {false,false,true },
                        {true ,false,false},
                        {false,true ,false},
                        {true ,false,true },
                        {true ,true ,false},
                        {false,true ,true },
                        {true ,true ,true },
                        };

void setup() {

  Serial.begin(9600);

	pinMode(pin1_led,OUTPUT);
  pinMode(pin2_led,OUTPUT);
	pinMode(pin3_led,OUTPUT);

	// In this line the WiFi_SSID and WIFI_PASS was stored in the env.h

  WiFi.begin(WIFI_SSID, WIFI_PASS);
  
	// This line is for Connecting to wifi

  Serial.println("Your device is Connecting...");
  while(WiFi.status() != WL_CONNECTED) {
    delay(700);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Your device is Connected to WiFi network and it's IP Address is: ");
  Serial.println(WiFi.localIP());
}

void loop() {

  //This line is for the checking the WiFi connection status

  if(WiFi.status()== WL_CONNECTED){

    HTTPClient http;
  
    // This line establishes a connection to the server
    http.begin(endpoint);
    
    for (int i = 0 ; i < 8; i++){
    
      String httpRequestData;

      StaticJsonDocument<1024> doc;

      // This line serialise JSON object into a string to be sent to the API

      doc["light_switch_1"] = pattern[i][0];
      doc["light_switch_2"] = pattern[i][1];
      doc["light_switch_3"] = pattern[i][2];


      // This line is to convert JSON document, doc, to string and copies it into httpRequestData

      serializeJson(doc, httpRequestData);

      // This line specifies the content-type header

      http.addHeader("X-API-Key",API_KEY);
      http.addHeader("Content-Type", "application/json");

      // Send HTTP PUT request
      int httpResponseCode = http.PUT(httpRequestData);
      String http_response;


      if (httpResponseCode>0) {
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);

        Serial.print("HTTP Response from server: ");
        http_response = http.getString();
        Serial.println(http_response);
      }
      else {
        Serial.print("Error code: ");
        Serial.println(httpResponseCode);
      }
      delay(2000);
    }
    http.end();
  }
}