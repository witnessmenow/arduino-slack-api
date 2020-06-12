/*******************************************************************
    Toggles between a presence of "away" and "auto" on your slack account. 
    It will toggle between the two every 30 seconds

    You will need a bearer token, see readme for more details

    You will also need to be on version 2.5 or higher of the ESP8266 core

    Parts:
    D1 Mini ESP8266 * - http://s.click.aliexpress.com/e/uzFUnIe

 *  * = Affilate

    If you find what I do usefuland would like to support me,
    please consider becoming a sponsor on Github
    https://github.com/sponsors/witnessmenow/


    Written by Brian Lough
    YouTube: https://www.youtube.com/brianlough
    Tindie: https://www.tindie.com/stores/brianlough/
    Twitter: https://twitter.com/witnessmenow
 *******************************************************************/

// ----------------------------
// Standard Libraries
// ----------------------------

#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <ArduinoSlack.h>
// Library for connecting to the Slack API

// Install from Github
// https://github.com/witnessmenow/arduino-slack-api

#include <ArduinoJson.h>
// Library used for parsing Json from the API responses

// Search for "Arduino Json" in the Arduino Library manager
// https://github.com/bblanchon/ArduinoJson

//------- Replace the following! ------

char ssid[] = "SSID";         // your network SSID (name)
char password[] = "password"; // your network password

#define SLACK_ACCESS_TOKEN "AAAAAAAAAABBBBBBBBBBBCCCCCCCCCCCDDDDDDDDDDD"


//------- ---------------------- ------

WiFiClientSecure client;
ArduinoSlack slack(client, SLACK_ACCESS_TOKEN);

unsigned long delayBetweenRequests = 30000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

bool isPresenceAway = false;

void setup() {

  Serial.begin(115200);

    // Set WiFi to station mode and disconnect from an AP if it was Previously
    // connected
    WiFi.mode(WIFI_STA);
    WiFi.disconnect();
    delay(100);

    // Attempt to connect to Wifi network:
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
    Serial.println("IP address: ");
    IPAddress ip = WiFi.localIP();
    Serial.println(ip);

    client.setFingerprint(SLACK_FINGERPRINT);

    // If you want to enable some extra debugging
    //slack._debug = true;
}

void loop() {
  if (millis() > requestDueTime)
    {
        if(isPresenceAway){
            if(slack.setPresence(SLACK_PRESENCE_AUTO)){
                Serial.println("Set presence to Auto");
            } else {
                Serial.println("Failed to set presence to Auto");
            }
        } else {
            if(slack.setPresence(SLACK_PRESENCE_AWAY)){
                Serial.println("Set presence to Away");
            } else {
                Serial.println("Failed to set presence to Away");
            }
        }
        isPresenceAway = !isPresenceAway;
        requestDueTime = millis() + delayBetweenRequests;
    }

}
