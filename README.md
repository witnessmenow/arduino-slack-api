# arduino-slack-api
An arduino library to wrap the Slack API

Currently the library supports updating your Presence and setting a Custom Status.

![img](https://i.imgur.com/UUIQj2P.jpg)

## Help support what I do!

I have created a lot of different Arduino libraries that I hope people can make use of. [If you enjoy my work, please consider becoming a Github sponsor!](https://github.com/sponsors/witnessmenow/)

### Getting Auth Token (this may change)

- Create a new Slack App [here](https://api.slack.com/apps)
- Name your app anything, set the "Development Slack Workspace" to the workspace you want to update your status in
- This will bring you to a new page, on the left hand-side there is a menu. Click "OAuth & Permissions"
- Go down to "Scopes", then under "User Token Scopes", click the "Add an OAuth Scope" button
- Add the scopes you need. Here is a list of supported features in this library and their required scopes:

| Enpoint        | scope           |
| ------------- |-------------|
| setPresence      | users:write |
| setCustomStatus      | users.profile:write |

- scroll up to the top of the page and click "install app to workspace"
- Click "Allow" on the next page
- You will now have an OAuth Access Token up the top of the page, this is uses as the SLACK_ACCESS_TOKEN
