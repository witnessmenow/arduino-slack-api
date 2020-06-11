# arduino-slack-api
An arduino library to wrap the Slack API


### Getting Auth Token (this may change)

- Create a new Slack App [here](https://api.slack.com/apps)
- Name your app anything, set the "Development Slack Workspace" to the workspace you want to update your status in
- This will bring you to a new page, on the left hand-side there is a menu. Click "OAuth & Permissions"
- Go down to "Scopes", then under "User Token Scopes", click the "Add an OAuth Scope" button
- start typing in "users.profile.write" and click on it when it pops up
- scroll up to the top of the page and click "install app to workspace"
- Click "Allow" on the next page
- You will now have an OAuth Access Token up the top of the page, this is uses as the SLACK_ACCESS_TOKEN