# Sane API

## Running it

`python3 sane_api.py`

## Accessing it

`127.0.0.1:5002/api`

## Documentation

SaneAPI consists of three sub-categories; *local*, *remote* and *youtube*.

### Local

#### Add subscription
Returns a youtube#channel resource.

Used to add a YouTube subscription locally.

##### HTTP request
`GET http://127.0.0.1:5002/api/v1/local/add_subscription`

##### Required parameters
Specify exactly one of the following parameters

| Parameter           | Type   | Description             | 
|---------------------|--------|-------------------------|
| username            | string | Title/Name of a channel |
| id                  | string | ID of a channel         |


### Remote
Requests to the YouTube API with some extra functionality added on.
#### Subscriptions
Returns a list of the user's subscriptions from YouTube API.

These subscriptions has been modified by attaching the `contentDetails` resource to each one.

##### HTTP request
`GET http://127.0.0.1:5002/api/v1/remote/subscriptions`

#### ~~Subfeed~~ [Not yet implemented]
Returns a youtube#channel resource.

Used to add a YouTube subscription locally.

##### HTTP request
`GET http://127.0.0.1:5002/api/v1/remote/subfeed`

#### Channel
Returns a `youtube#channel` resource.

##### Required parameters
Specify exactly one of the following parameters

| Parameter           | Type   | Description             | 
|---------------------|--------|-------------------------|
| username            | string | Title/Name of a channel |
| id                  | string | ID of a channel         |

##### HTTP request
`GET http://127.0.0.1:5002/api/v1/remote/channel`

### YouTube
Pass-through kwargs directly to the YouTube API at https://www.googleapis.com/youtube/v3/

#### Activities
##### 