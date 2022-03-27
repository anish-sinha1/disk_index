#ifndef B_TREE_SONG_H
#define B_TREE_SONG_H

#include <cslice.h>

enum F_STAT {
    WRITE_SUCCESS = 1, WRITE_FAIL = 2, READ_SUCCESS = 3, READ_FAIL = 4, FMT_SUCCESS = 5, FMT_FAIL = 6
};

struct Song {
    char title[46], artist[46], lyrics[688];
    double length;
    int likes;
};

struct IdxHeader {
    int node_count, root_loc;
    size_t page_size;
};

struct Song *song(char *, char *, char *, double, int);

int compare_song(const void *, const void *);

enum F_STAT write_song(struct Song *);
enum F_STAT read_song(int, struct Song *);
enum F_STAT fmt_idx(void);
enum F_STAT read_idx_header(struct IdxHeader *ih);
enum F_STAT fmt_table(char *);
enum F_STAT gen_idx(void);

Slice *read_block(int);

void serialize_song(struct Song *);
void clear_data(void);
void clear_idx(void);

#endif