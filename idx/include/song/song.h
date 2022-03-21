#ifndef B_TREE_SONG_H
#define B_TREE_SONG_H

#include <cslice.h>
#include "common.h"

struct Song;

struct Song *song(char *, char *, char *, double, int);

enum F_STAT write_song(struct Song *);
enum F_STAT read_song(int, struct Song *);
Slice *read_block(int);

void serialize_song(struct Song *);
void clear_data();

void test();

#endif