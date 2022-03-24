#include <cslice.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

#include "common.h"

struct Song *song(char *title, char *artist, char *lyrics, double length, int likes) {
    struct Song *s = malloc(sizeof(struct Song));
    strncpy(s->title, title, 46);
    strncpy(s->artist, artist, 46);
    strncpy(s->lyrics, lyrics, 688);
    s->likes = likes;
    s->length = length;
    return s;
}

int compare_song(const void *first, const void *second) {
    return -strcmp(((struct Song *) first)->title, ((struct Song *) second)->title);
}

// write a song to the binary data file
enum F_STAT write_song(struct Song *s) {
    if (!s) return WRITE_FAIL;
    int fd = open(DATA_PATH, O_RDWR);
    {
        struct stat buf;
        fstat(fd, &buf);
        pwrite(fd, s, sizeof(struct Song), buf.st_size);
    }
    close(fd);
    return WRITE_SUCCESS;
}

// read a song from a binary data file
enum F_STAT read_song(int pos, struct Song *s) {
    if (pos < 0) return READ_FAIL;
    int fd = open(DATA_PATH, O_RDONLY);
    {
        struct stat buf;
        fstat(fd, &buf);
        if (pos * sizeof(struct Song) > buf.st_size) return READ_FAIL;
        pread(fd, s, sizeof(struct Song), (off_t) sizeof(struct Song) * pos);
    }
    close(fd);
    return READ_SUCCESS;
}

void serialize_song(struct Song *s) {
    printf("{\n");
    printf("\ttitle: %s\n", s->title);
    printf("\tartist: %s\n", s->artist);
    printf("\tlyrics: %s\n", s->lyrics);
    printf("\tlikes: %d\n", s->likes);
    printf("\tlength: %f\n", s->length);
    printf("}\n");
}

// reads a block of data into memory and organizes it into a nice, processable slice
Slice *read_block(int page) {
    if (page < 0) return slice(1);
    int fd = open(DATA_PATH, O_RDONLY);
    Slice *memory_block = slice(10);
    {
        struct stat buf;
        fstat(fd, &buf);
        if (buf.st_size < page * PAGE_SIZE) return slice(1);
        int record_count = (int) ((buf.st_size - (page * PAGE_SIZE)) / sizeof(struct Song));
        char block[PAGE_SIZE];
        pread(fd, block, PAGE_SIZE, page * PAGE_SIZE);
        for (int i = 0; i < record_count; i++) {
            struct Song *s = malloc(sizeof(struct Song));
            memcpy(s, block + sizeof(struct Song) * i, sizeof(struct Song));
            push(memory_block, s);
        }
    }
    close(fd);
    return memory_block;
}

void clear_data() {
    truncate(DATA_PATH, 0);
}