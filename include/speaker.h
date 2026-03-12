#ifndef SPEAKER_H
#define SPEAKER_H

typedef unsigned int uint32_t;

void speaker_play(uint32_t hz);
void speaker_stop();
void speaker_beep(uint32_t hz, uint32_t ms);

#endif