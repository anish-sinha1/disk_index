#ifndef B_TREE_SONG_H
#define B_TREE_SONG_H

#include "common.h"

struct Song;

struct Song *song(const char *, const char *, int);
int compare_songs(struct Song *, struct Song *);
void serialize_song(struct Song *);
void clear_songs(void);
enum F_STAT write_song(struct Song *);
struct Song *read_song(int);

#endif