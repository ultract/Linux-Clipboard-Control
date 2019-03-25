#!/bin/bash

# gtk 2.0
gcc clipboard_control.c -o clipboard_control `pkg-config --libs --cflags gtk+-2.0`

# gtk 3.0
#gcc clipboard_control.c -o clipboard_control `pkg-config --libs --cflags gtk+-3.0`

