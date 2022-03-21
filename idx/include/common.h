#ifndef B_TREE_COMMON_H
#define B_TREE_COMMON_H

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>

#define DATA_PATH "/Users/anishsinha/Home/cprojects/b+tree/data/tables/songs/data.bin"
#define PAGE_SIZE 8000

// exactly 800 bytes for convenience
struct Song {
    char title[46], artist[46], lyrics[688];
    double length;
    int likes;
};

enum F_STAT {
    WRITE_SUCCESS = 1, WRITE_FAIL = 2, READ_SUCCESS = 3, READ_FAIL = 4
};

struct Header {
    bool full;
    int bytes_remaining;
};

#endif