# Sane++
A Sane Subscriptions Feed C++ rewrite.

[![Build Status](https://api.travis-ci.org/BluABK/sanepp.svg?branch=master)](https://travis-ci.org/BluABK/sanepp)
[![GitHub](https://img.shields.io/github/license/bluabk/sanepp.svg)](https://raw.githubusercontent.com/BluABK/sanepp/master/LICENSE.md)
[![GitHub issues](https://img.shields.io/github/issues/bluabk/sanepp.svg)](https://github.com/BluABK/sanepp/issues)
[![Discord](https://img.shields.io/discord/499274159839379473.svg?style=popout)](https://discord.gg/7KcvPt4)
![GitHub repo size](https://img.shields.io/github/repo-size/bluabk/sanepp.svg?style=popout)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/bluabk/sanepp.svg)


## What is this?
Simply put the core function is to create a more reliable subscriptions feed than YouTube's.

Sane++ grabs the "uploaded videos" playlist for all channels you're subscribed to and generates a feed based on that, bypasing the infamous algorithm. 

## How to compile
  1. `mkdir build && cd build`
  2. `cmake ../src/ && cmake --build .`
  3. `cd ..` (avoids relative paths in later instructions)

## Prerequisite YouTube OAuth2 authentication
1. Go through method A or B and then perform the following further steps:
2. Run the CLI OAuth authentication command: `build/bin/sane++_cli auth-oauth2`.
3. Open the link it produced in a web browser and go through the authentication process.

### Method A) Using the public YouTube API credentials
1. Copy `config.json.sample` to `config.json`

### Method B) Using custom YouTube API credentials
1. Copy `config.json.sample` to `config.json`
2. Change `client_id`, `client_secret` and `api_key` according to the ones Google API provided you.

## Running it

### Command line interface
`build/bin/sane++_cli`

Supports commands as params from the command line (e.g. `build/bin/sane++_cli auth-oauth2`)

### GUI
Not yet implemented.

## FAQ
## Why rewrite the software?
Aside from the obvious performance enhancements and reduced complexity I decided to rewrite it in C++ because I kept running into nonsense issues with libQt segfaulting due to PyQt, and since it's C++ wrapped objects in python... GLHF debugging...

This and other gripes eventually led to this rewrite.
