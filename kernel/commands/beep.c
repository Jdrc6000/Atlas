#include "speaker.h"
#include "vga.h"

static int myatoi(const char *s) {
    int n = 0;
    while (*s >= '0' && *s <= '9')
        n = n * 10 + (*s++ - '0');
    return n;
}

int cmd_beep(int argc, char **argv) {
    uint32_t hz = 1000;
    uint32_t ms = 300;

    if (argc >= 2) hz = myatoi(argv[1]);
    if (argc >= 3) ms = myatoi(argv[2]);

    if (hz < 20 || hz > 20000) {
        vga_print("beep: frequency must be 20-20000 hz\n");
        return 1;
    }

    speaker_beep(hz, ms);
    return 0;
}