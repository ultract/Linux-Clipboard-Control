# Linux-Clipboard-Control
Simply control the clipboard in a desktop linux environment (Allow / Disallow)

### Prerequisite
- GCC Compiler
- Gtk 2.0 or Gtk 3.0 Library 

### Build
> $ gcc clipboard_control.c -o clipboard_control `pkg-config --libs --cflags gtk+-2.0`
> or
> $ gcc clipboard_control.c -o clipboard_control `pkg-config --libs --cflags gtk+-3.0

### Run
> $ ./clipboard_control

### Debug
> $ G_MESSAGES_DEBUG=all ./clipboard_control
