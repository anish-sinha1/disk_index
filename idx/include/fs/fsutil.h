#ifndef B_TREE_FSUTIL_H
#define B_TREE_FSUTIL_H

struct Song;

enum IO_STAT {
    WRITE_SUCCESS = 1, READ_SUCCESS = 2, WRITE_FAIL = 3, READ_FAIL = 4
};

enum IO_STAT write_record(struct Song *);

struct Song *song(const char *, const char *, unsigned int);

#endif