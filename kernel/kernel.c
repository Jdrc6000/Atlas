void kernel_main() {
    char *video = (char *)0xB8000;

    video[0] = 'A';
    video[1] = 0x07;

    video[2] = 't';
    video[3] = 0x07;

    video[4] = 'l';
    video[5] = 0x07;

    video[6] = 'a';
    video[7] = 0x07;

    video[8] = 's';
    video[9] = 0x07;

    while (1)
        ;
}