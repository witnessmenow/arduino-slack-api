/*******************************************************************
    Sets a custom status on your slack account using 
    Arduino WifiNINA boards (e.g. Nano IoT)  
    
    It will update every 30 seconds

    You will need a bearer token, see readme for more details

    Parts:
    Arduino Nano 33 IOT - https://store.arduino.cc/arduino-nano-33-iot

 *  * = Affiliate

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
#include <SPI.h>

// ----------------------------
// Additional Libraries - each one of these will need to be installed.
// ----------------------------

#include <WiFiNINA.h>
// Library for using network features of the official Arudino
// Wifi Boards (MKR WiFi 1010, Nano 33 IOT etc)

// Search for "nina" in the Arduino Library Manager
// https://github.com/arduino-libraries/WiFiNINA

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

int status = WL_IDLE_STATUS;

WiFiSSLClient client;
ArduinoSlack slack(client, SLACK_ACCESS_TOKEN);

unsigned long delayBetweenRequests = 30000; // Time between requests (1 minute)
unsigned long requestDueTime;               //time when request due

bool firstStatus = true;

void setup()
{
    //Initialize serial and wait for port to open:
    Serial.begin(9600);
    while (!Serial)
    {
        ; // wait for serial port to connect. Needed for native USB port only
    }

    // check for the WiFi module:
    if (WiFi.status() == WL_NO_MODULE)
    {
        Serial.println("Communication with WiFi module failed!");
        // don't continue
        while (true)
            ;
    }

    String fv = WiFi.firmwareVersion();
    if (fv < "1.0.0")
    {
        Serial.println("Please upgrade the firmware");
    }

    // attempt to connect to WiFi network:
    while (status != WL_CONNECTED)
    {
        Serial.print("Attempting to connect to SSID: ");
        Serial.println(ssid);
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, password);

        // wait 10 seconds for connection:
        delay(10000);
    }
    Serial.println("Connected to wifi");
    printWiFiStatus();

    // If you want to enable some extra debugging
    // uncomment the "#define SLACK_ENABLE_DEBUG" in ArduinoSlack.h
}

void displayProfile(SlackProfile profile)
{
    if (!profile.error)
    {
        Serial.println("--------- Profile ---------");

        Serial.print("Display Name: ");
        Serial.println(profile.displayName);

        Serial.print("Status Text: ");
        Serial.println(profile.statusText);

        Serial.print("Status Emoji: ");
        Serial.println(profile.statusEmoji);

        Serial.print("Status Expiration: ");
        Serial.println(profile.statusExpiration);

        Serial.println("------------------------");
    }
    else
    {
        Serial.println("error getting profile");
    }
}

void loop()
{
    if (millis() > requestDueTime)
    {
        SlackProfile profile;
        if (firstStatus)
        {
            profile = slack.setCustomStatus("I am the first status", ":sparkling_heart:");
        }
        else
        {
            profile = slack.setCustomStatus("I am the second status", ":v:");
            // There is an optional third parameter which takes a Unix timestamp for
            // when this custom status expires:
            // slack.setCustomStatus("I am the second status", ":v:", 1532627506);
        }
        firstStatus = !firstStatus;
        displayProfile(profile);
        requestDueTime = millis() + delayBetweenRequests;
    }
}

void printWiFiStatus()
{
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());

    // print your board's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);

    // print the received signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
}
