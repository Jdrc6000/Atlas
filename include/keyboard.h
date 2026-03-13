#ifndef KEYBOARD_H
#define KEYBOARD_H

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

#define KEY_UP 0x01
#define KEY_DOWN 0x02
#define KEY_LEFT 0x03
#define KEY_RIGHT 0x04

void keyboard_init();
char keyboard_poll();
void keyboard_push(char c);
char keyboard_getchar();
int keyboard_haschar();

#endif