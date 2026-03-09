#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60 // returns scancode
#define KEYBOARD_STATUS_PORT 0x64 // returns 0=not ready, 1=ready

void keyboard_init();
char keyboard_poll();

#endif