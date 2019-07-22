# Sane++
A Sane Subscription Feed C++ rewrite.

[![Build Status](https://api.travis-ci.org/BluABK/sanepp.svg?branch=master)](https://travis-ci.org/BluABK/sanepp)
[![GitHub](https://img.shields.io/github/license/bluabk/sanepp.svg)](https://raw.githubusercontent.com/BluABK/sanepp/master/LICENSE.md)
[![GitHub issues](https://img.shields.io/github/issues/bluabk/sanepp.svg)](https://github.com/BluABK/sanepp/issues)
[![Discord](https://img.shields.io/discord/499274159839379473.svg?style=popout)](https://discord.gg/7KcvPt4)
![GitHub repo size](https://img.shields.io/github/repo-size/bluabk/sanepp.svg?style=popout)
![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/bluabk/sanepp.svg)


## What is this?
Simply put the core function is to create a more reliable subfeed than YouYube's.

Sane++ grabs the "uploaded videos" playlist for all channels you're subscribed to and generates a feed based on that, bypasing the infamous algorithm. 

## How to compile
  1. `mkdir build && cd build`
  2. `cmake ../src/ && cmake --build .`
  3. `cd ..` (avoids relative paths in later instructions)

## Running it
### First time running it?
#### Prerequisite SaneAPI authentication caching.
It is currently necesarry to *prime* the SaneAPI's YouTube API keys and OAuth2 tokens, which will then be saved to disk in future.

  1. `python3 src/sane-api/sane_api.py`
  2. Make a request that triggers OAuth2 verification like `http://127.0.0.1:5002/api/v1/remote/subscriptions`
  3. Open the auth URL in browser (ignore the unsafe warning) and paste it into the command prompt.

### Running the actual thing
If SaneAPI is not already running, run `python3 src/sane-api/sane_api.py`
#### Command line interface
`build/bin/Sane++`

## FAQ
### Why rewrite the software?
Aside from the obvious performance enhancements and reduced complexity I decided to rewrite it in C++ because I kept running into nonsense issues with libQt segfaulting due to PyQt, and since it's C++ wrapped objects in python... GLHF debugging...

This and other gripes eventually led to this rewrite.
### Why do I need to run this inconvenient SaneAPI?
The SaneAPI is remnants of the old python version of Sane, which is sadly currently necessary to drag with us because Google seems to have discontinued their C++ API support.
