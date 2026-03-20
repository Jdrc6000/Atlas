#include "music.h"
#include "speaker.h"
#include "irq.h"

static void music_wait(uint32_t ms) {
    uint32_t ticks_needed = (ms * TICKS_PER_SEC) / 1000;

    if (ticks_needed == 0)
        ticks_needed = 1;
    
    uint32_t start = irq_get_ticks();

    while (irq_get_ticks() - start < ticks_needed) ;
}

void music_play(const note_t *song, uint32_t len) {
    for (uint32_t i = 0; i < len; i++) {
        if (song[i].freq == 0)
        {
            speaker_stop();
            music_wait(song[i].ms);
        } else {
            speaker_beep(song[i].freq, song[i].ms);
        }
        
        speaker_stop();
        music_wait(20);
    }
}