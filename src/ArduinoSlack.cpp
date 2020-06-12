/*
Copyright (c) 2020 Brian Lough. All right reserved.

ArduinoSlack - An Arduino library to wrap the Slack API

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.
This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
Lesser General Public License for more details.
You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301 USA
*/

#include "ArduinoSlack.h"

ArduinoSlack::ArduinoSlack(Client &client, char *bearerToken)
{
    this->client = &client;
    this->_bearerToken = bearerToken;
}

int ArduinoSlack::makePostRequest(char *command, char *body, char *contentType)
{
    client->flush();
    client->setTimeout(SLACK_TIMEOUT);
    if (!client->connect(SLACK_HOST, portNumber))
    {
        Serial.println(F("Connection failed"));
        return false;
    }

    // give the esp a breather
    yield();

    // Send HTTP request
    client->print(F("POST "));
    client->print(command);
    client->println(F(" HTTP/1.1"));

    //Headers
    client->print(F("Host: "));
    client->println(SLACK_HOST);

    client->println(F("Accept: application/json"));
    client->print(F("Content-Type: "));
    client->println(contentType);

    client->print(F("Authorization: Bearer "));
    client->println(_bearerToken);

    client->println(F("Cache-Control: no-cache"));

    client->print(F("Content-Length: "));
    client->println(strlen(body));

    client->println();

    //send Data here?
    client->print(body);

    if (client->println() == 0)
    {
        Serial.println(F("Failed to send request"));
        return false;
    }

    int statusCode = getHttpStatusCode();
    skipHeaders();
    return statusCode;
}

bool ArduinoSlack::setPresence(char *presence){
    char command[100];
    sprintf(command, SLACK_USERS_SET_PRESENCE_ENDPOINT, presence);
    if (_debug)
    {
        Serial.println(command);
    }

    // Get from https://arduinojson.org/v6/assistant/
    const size_t bufferSize = 1000;
    bool okStatus = false;
    if (makePostRequest(command, "", "text/plain") == 200)
    {
        // Allocate DynamicJsonDocument
        DynamicJsonDocument doc(bufferSize);

        // Parse JSON object
        DeserializationError error = deserializeJson(doc, *client);
        if (!error)
        {
            if(_debug){
                char temp[500];
                serializeJson(doc, temp);
                Serial.println(temp);
            }
            okStatus = doc["ok"];
            if(!okStatus){
                if(doc.containsKey("error")){
                    const char* errorMsg = doc["error"];
                    Serial.print(F("Got the following error: "));
                    Serial.println(errorMsg);
                } else {
                    Serial.print(F("Unkown Error"));
                }
            }

        }
        else
        {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
        }
    }
    closeClient();
    return okStatus;
}

SlackProfile ArduinoSlack::setCustomStatus(char *text, char *emoji, int expiration ){
    char body[300];
    sprintf(body, setEnpointBody, text, emoji, expiration);
    if (_debug)
    {
        Serial.println(body);
    }

    // Get from https://arduinojson.org/v6/assistant/
    const size_t bufferSize = profileBufferSize;
    SlackProfile profile;
    // This flag will get cleared if all goes well
    profile.error = true;
    if (makePostRequest(SLACK_USERS_PROFILE_SET_ENDPOINT, body) == 200)
    {
        // Allocate DynamicJsonDocument
        DynamicJsonDocument doc(bufferSize);

        // Parse JSON object
        DeserializationError error = deserializeJson(doc, *client);
        if (!error)
        {
            JsonObject profileObj = doc["profile"];

            profile.displayName = (char *) profileObj["display_name"].as<char *>(); 
            profile.statusText = (char *) profileObj["status_text"].as<char *>(); 
            profile.statusEmoji = (char *) profileObj["status_emoji"].as<char *>(); 

            profile.statusExpiration = profileObj["status_expiration"].as<int>();

            profile.error = false;
        }
        else
        {
            Serial.print(F("deserializeJson() failed with code "));
            Serial.println(error.c_str());
        }
    }
    closeClient();
    return profile;
}


void ArduinoSlack::skipHeaders()
{
    // Skip HTTP headers
    char endOfHeaders[] = "\r\n\r\n";
    if (!client->find(endOfHeaders))
    {
        Serial.println(F("Invalid response"));
        return;
    }

    // Was getting stray characters between the headers and the body
    // This should toss them away
    while (client->available() && client->peek() != '{')
    {
        char c = 0;
        client->readBytes(&c, 1);
        if (_debug)
        {
            Serial.print("Tossing an unexpected character: ");
            Serial.println(c);
        }
    }
}

int ArduinoSlack::getHttpStatusCode()
{
    // Check HTTP status
    if(client->find("HTTP/1.1")){
        int statusCode = client->parseInt();
        return statusCode;
    } 

    return -1;
}

void ArduinoSlack::closeClient()
{
    if (client->connected())
    {
        if (_debug)
        {
            Serial.println(F("Closing client"));
        }
        client->stop();
    }
}