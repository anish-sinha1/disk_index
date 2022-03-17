#include <cslice.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "common.h"

struct Song {
    char title[50];
    char artist[50];
    int likes;
};

struct Song *song(const char *title, const char *artist, int likes) {
    struct Song *s = malloc(sizeof(struct Song));
    strncpy(s->title, title, 50);
    strncpy(s->artist, artist, 50);
    s->likes = likes;
    return s;
}

int compare_songs(struct Song *first, struct Song *second) {
    return strcmp(first->title, second->title);
}

// the "table" we write to is just one large binary file. there's no "pages" or anything special. literally, structs
// are just appended to the end of the file. this isn't super efficient since the whole table has to be read into
// memory, but we don't care about that shit. we just want to test a b+tree index. When I make a Lehman and Yao Index, I
// might put more effort into this part
enum F_STAT write_song(struct Song *s) {
    if (!s) return WRITE_ERROR;
    int fd = open(SONG_TABLE_FILE, O_RDWR);
    {
        struct stat buf;
        fstat(fd, &buf);
        pwrite(fd, s, sizeof(struct Song), buf.st_size);
    }
    close(fd);
    return WRITE_SUCCESS;
}

struct Song *read_song(int pos) {
    int fd = open(SONG_TABLE_FILE, O_RDONLY);
    struct Song *s = malloc(sizeof(struct Song));
    {
        struct stat buf;
        fstat(fd, &buf);
        // there's probably a more user-friendly way to do this but honestly fuck that lol. here's a null pointer. it's
        // your own fault for passing in stupid input.
        if (pos * sizeof(struct Song) > buf.st_size || pos < 0) return NULL;
        pread(fd, s, sizeof(struct Song), (off_t) sizeof(struct Song) * pos);
    }
    close(fd);
    return s;
}

void clear_songs() {
    truncate(SONG_TABLE_FILE, 0);
}

void serialize_song(struct Song *s) {
    printf("{\n");
    printf("\ttitle: %s\n", s->title);
    printf("\tartist: %s\n", s->artist);
    printf("\tlikes: %d\n", s->likes);
    printf("}\n");
}