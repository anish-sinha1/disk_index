#ifndef B_TREE_FSUTIL_H
#define B_TREE_FSUTIL_H

#include "common.h"

struct Song;

enum IO_STAT write_record(struct Song *);

struct Song *song(const char *, const char *, unsigned int);

#endif