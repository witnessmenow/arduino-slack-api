/*******************************************************************
    Toggles between a presence of "away" and "auto" on your slack account. 
    It will toggle between the two every 30 seconds

    (FYI: You can't set to "online"- https://api.slack.com/methods/users.setPresence)

    You will need a bearer token, see readme for more details

    Parts:
    ESP32 D1 Mini style Dev board* - http://s.click.aliexpress.com/e/C6ds4my

 *  * = Affilate

    If you find what I do useful and would like to support me,
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

#include <WiFi.h>
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

// including a "slack_server_cert" variable
// header is included as part of the ArduinoSlack libary
#include <ArduinoSlackCert.h>

WiFiClientSecure client;
ArduinoSlack slack(client, SLACK_ACCESS_TOKEN);

unsigned long delayBetweenRequests = 30000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

bool isPresenceAway = false;

void setup()
{

    Serial.begin(115200);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    Serial.println("");

    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    client.setCACert(slack_server_cert);

    // If you want to enable some extra debugging
    // uncomment the "#define SLACK_ENABLE_DEBUG" in ArduinoSlack.h
}

void loop()
{
    if (millis() > requestDueTime)
    {
        if (isPresenceAway)
        {
            if (slack.setPresence(SLACK_PRESENCE_AUTO))
            {
                Serial.println("Set presence to Auto");
            }
            else
            {
                Serial.println("Failed to set presence to Auto");
            }
        }
        else
        {
            if (slack.setPresence(SLACK_PRESENCE_AWAY))
            {
                Serial.println("Set presence to Away");
            }
            else
            {
                Serial.println("Failed to set presence to Away");
            }
        }
        isPresenceAway = !isPresenceAway;
        requestDueTime = millis() + delayBetweenRequests;
    }
}
