#ifndef MUSIC_H
#define MUSIC_H

typedef unsigned int uint32_t;

typedef struct {
    uint32_t freq;
    uint32_t ms;
} note_t;

void music_play(const note_t *song, uint32_t len);

#endif