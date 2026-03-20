#include "music.h"
#include "notes.h"
#include "vga.h"
#include "kstring.h"

#define N(f, d) {f, d}

#define WH 1600 // whole
#define HF 800 // half
#define QT 400 // quarter
#define ET 200 // eighth
#define ST 100 // sixteenth
#define DQT 600 // dotted quarter (quarter + eighth)
#define DHF 1200 // dotted half

// Tetris Theme
static const note_t tetris[] = {
    N(E5, QT),
    N(B4, ET),
    N(C5, ET),
    N(D5, QT),
    N(C5, ET),
    N(B4, ET),
    N(A4, QT),
    N(A4, ET),
    N(C5, ET),
    N(E5, QT),
    N(D5, ET),
    N(C5, ET),
    N(B4, DQT),
    N(C5, ET),
    N(D5, QT),
    N(E5, QT),
    N(C5, QT),
    N(A4, QT),
    N(A4, HF),
    N(REST, ET),
    N(D5, DQT),
    N(F5, ET),
    N(A5, QT),
    N(G5, ET),
    N(F5, ET),
    N(E5, DQT),
    N(C5, ET),
    N(E5, QT),
    N(D5, ET),
    N(C5, ET),
    N(B4, QT),
    N(B4, ET),
    N(C5, ET),
    N(D5, QT),
    N(E5, QT),
    N(C5, QT),
    N(A4, QT),
    N(A4, QT),
    N(REST, QT),
    // repeat
    N(E5, QT),
    N(B4, ET),
    N(C5, ET),
    N(D5, QT),
    N(C5, ET),
    N(B4, ET),
    N(A4, QT),
    N(A4, ET),
    N(C5, ET),
    N(E5, QT),
    N(D5, ET),
    N(C5, ET),
    N(B4, DQT),
    N(C5, ET),
    N(D5, QT),
    N(E5, QT),
    N(C5, QT),
    N(A4, QT),
    N(A4, HF),
};

// Super Mario Bros Theme
static const note_t mario[] = {
    N(E5, ET),
    N(E5, ET),
    N(REST, ET),
    N(E5, ET),
    N(REST, ET),
    N(C5, ET),
    N(E5, QT),
    N(G5, QT),
    N(REST, QT),
    N(G4, QT),
    N(REST, QT),
    N(C5, DQT),
    N(G4, ET),
    N(REST, QT),
    N(E4, DQT),
    N(A4, QT),
    N(B4, QT),
    N(AS4, ET),
    N(A4, QT),
    N(G4, ET),
    N(E5, ET),
    N(G5, ET),
    N(A5, QT),
    N(F5, ET),
    N(G5, ET),
    N(REST, ET),
    N(E5, QT),
    N(C5, ET),
    N(D5, ET),
    N(B4, DQT),
    N(C5, DQT),
    N(G4, ET),
    N(REST, QT),
    N(E4, DQT),
    N(A4, QT),
    N(B4, QT),
    N(AS4, ET),
    N(A4, QT),
    N(G4, ET),
    N(E5, ET),
    N(G5, ET),
    N(A5, QT),
    N(F5, ET),
    N(G5, ET),
    N(REST, ET),
    N(E5, QT),
    N(C5, ET),
    N(D5, ET),
    N(B4, DQT),
};

// Happy Birthday
static const note_t happy_birthday[] = {
    N(C4, ET),
    N(C4, ST),
    N(D4, QT),
    N(C4, QT),
    N(F4, QT),
    N(E4, HF),
    N(C4, ET),
    N(C4, ST),
    N(D4, QT),
    N(C4, QT),
    N(G4, QT),
    N(F4, HF),
    N(C4, ET),
    N(C4, ST),
    N(C5, QT),
    N(A4, QT),
    N(F4, QT),
    N(E4, QT),
    N(D4, QT),
    N(AS4, ET),
    N(AS4, ST),
    N(A4, QT),
    N(F4, QT),
    N(G4, QT),
    N(F4, HF),
};

// Imperial March
static const note_t imperial_march[] = {
    N(A4, QT),
    N(A4, QT),
    N(A4, QT),
    N(F4, ET),
    N(C5, ET),
    N(A4, QT),
    N(F4, ET),
    N(C5, ET),
    N(A4, HF),
    N(E5, QT),
    N(E5, QT),
    N(E5, QT),
    N(F5, ET),
    N(C5, ET),
    N(GS4, QT),
    N(F4, ET),
    N(C5, ET),
    N(A4, HF),
    N(A5, QT),
    N(A4, ET),
    N(A4, ET),
    N(A5, QT),
    N(GS5, ET),
    N(G5, ET),
    N(FS5, ET),
    N(F5, ET),
    N(FS5, ET),
    N(REST, ET),
    N(AS4, ET),
    N(DS5, QT),
    N(D5, ET),
    N(CS5, ET),
    N(C5, ET),
    N(B4, ET),
    N(C5, ET),
    N(REST, ET),
    N(F4, ET),
    N(GS4, QT),
    N(F4, ET),
    N(A4, ET),
    N(C5, QT),
    N(A4, ET),
    N(C5, ET),
    N(E5, HF),
    N(A5, QT),
    N(A4, ET),
    N(A4, ET),
    N(A5, QT),
    N(GS5, ET),
    N(G5, ET),
    N(FS5, ET),
    N(F5, ET),
    N(FS5, ET),
    N(REST, ET),
    N(AS4, ET),
    N(DS5, QT),
    N(D5, ET),
    N(CS5, ET),
    N(C5, ET),
    N(B4, ET),
    N(C5, ET),
    N(REST, ET),
    N(F4, ET),
    N(GS4, QT),
    N(F4, ET),
    N(C5, ET),
    N(A4, QT),
    N(F4, ET),
    N(C5, ET),
    N(A4, HF),
};

typedef struct {
    const char *name;
    const note_t *notes;
    unsigned int len;
} song_t;

#define SONG(name, arr) {name, arr, sizeof(arr) / sizeof((arr)[0])}

static const song_t songs[] = {
    SONG("tetris", tetris),
    SONG("mario", mario),
    SONG("birthday", happy_birthday),
    SONG("imperial", imperial_march),
};

#define NUM_SONGS (sizeof(songs) / sizeof(songs[0]))

int cmd_play(int argc, char **argv) {
    if (argc < 2) {
        vga_print("usage: play <song>\n");
        vga_print("songs: tetris, mario, birthday, imperial\n");
        return 1;
    }

    for (unsigned int i = 0; i < NUM_SONGS; i++) {
        if (kstrcmp(songs[i].name, argv[1]))
        {
            vga_print("playing: ");
            vga_print(songs[i].name);
            vga_putchar('\n');
            music_play(songs[i].notes, songs[i].len);
            return 0;
        }
    }

    vga_print("play: unknown song '");
    vga_print(argv[1]);
    vga_print("'\nsongs: tetris, mario, birthday, imperial\n");

    return 1;
}