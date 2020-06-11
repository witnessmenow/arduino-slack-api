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

#ifndef ArduinoSlack_h
#define ArduinoSlack_h

#include <Arduino.h>
#include <ArduinoJson.h>
#include <Client.h>

#define SLACK_HOST "slack.com"
// Fingerprint correct as of June 11th 2020
#define SLACK_FINGERPRINT "C1 0D 53 49 D2 3E E5 2B A2 61 D5 9E 6F 99 0D 3D FD 8B B2 B3"
#define SLACK_TIMEOUT 2000


#define SLACK_USERS_PROFILE_SET_ENDPOINT "/api/users.profile.set"

struct SlackProfile
{
    char *displayName;
    char *statusText;
    char *statusEmoji;
    int statusExpiration;
    bool error;
};

class ArduinoSlack
{
  public:
    ArduinoSlack(Client &client, char *bearerToken);

    int makePostRequest(char *command, char *body);
    SlackProfile setCustomStatus(char *text, char *emoji, int expiration = 0);
    int portNumber = 443;
    int profileBufferSize = 10000;
    bool _debug = false;
    Client *client;

  private:
    char *_bearerToken;
    int getHttpStatusCode();
    void skipHeaders();
    void closeClient();

    const char *setEnpointBody = 
    R"({"profile": { "status_text": "%s","status_emoji": "%s","status_expiration": %d}})"
    ;
};

#endif