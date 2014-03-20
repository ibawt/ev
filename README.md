EV
====
[![Build Status](https://travis-ci.org/ibawt/ev.png?branch=master)](https://travis-ci.org/ibawt/ev)
Yet Another 2D C library

I didn't see many simple 2D C gaming libraries and I didn't want to use c++.

## Features
- sprite batching with all transformations done in a vertex shader
- raw old school C

## External Depenencies
- CMake >= 2.8
- a c/c++ compiler (C++ only for Box2D)
- SDL2.0 and SDLImage 2.0
- luajit

## Included libraries
- jannson
- Box2D
- uthash
- cmocka

## HOWTO compile
# OSX
- grab SDL2.0 and SDLImage 2.0 and stuff them into /Library/Frameworks
- install cmake from somewhere
- make a build/ directory and run "cmake <path_to_build>"
