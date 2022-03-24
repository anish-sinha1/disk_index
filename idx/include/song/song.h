#ifndef B_TREE_SONG_H
#define B_TREE_SONG_H

#include <cslice.h>
#include "common.h"

struct Song;

struct Song *song(char *, char *, char *, double, int);

int compare_song(const void *, const void *);

enum F_STAT write_song(struct Song *);
enum F_STAT read_song(int, struct Song *);
Slice *read_block(int);

void serialize_song(struct Song *);
void clear_data();

#endif