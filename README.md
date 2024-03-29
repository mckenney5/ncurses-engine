# NCURSES-ENGINE

## Description

This library makes using ncurses(3) easier to use with built in functions.

Here is a program using the engine.

![UserInterfaceModule screenshot](https://github.com/mckenney5/ncurses-engine/raw/master/screenshot.png)

## Project Meta
This software is in a **working alpha** state.
The software may not work and may have drastic changes in the future.

## How to Include

To add 'nengine.h', you need the ncurses(3) library (or libncurses5-dev on some distros). 
In your program, include the engine.h file via `#include "nengine.h"`.
Note: remember when running `gcc(1)`, you need to specify 
the library with the `-l` flag (ex `-lncurses`).

An example would be:  `gcc -lncurses your-program.c`

## Files in the Repository

LICENSE -					The license of the software. Tells you what you can and cant do with it. MIT

ROADMAP -					Project goals that lead to different versions

TODO -						Goals of this project

/doc/EngineRef.A0.0.01.pdf -A reference for using the engine.h file. Read only PDF

/doc/EngineRef.odt -		A reference for using the engine.h file. Editable ODT

/example/example1.c -		A simple example that shows off some features of the engine

/src/nengine.h -				Handles creating menus, leans heavily on ncurses(3)

