#include "common.h"
#include "song/song.h"

#include <cslice.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

enum F_STAT fmt_idx() {
    truncate(IDX_PATH, 0);
    int fd = open(IDX_PATH, O_RDWR);
    struct IdxHeader *ih = idx_header(0, PAGE_SIZE, -1);
    if (!ih) return FMT_FAIL;
    {
        pwrite(fd, ih, sizeof(struct IdxHeader), 0);
    }
    close(fd);
    return FMT_SUCCESS;
}

enum F_STAT insert_key(struct Song *s) {
    if (!s) return WRITE_FAIL;
    int fd = open(IDX_PATH, O_RDWR);
    {
        struct NodeHeader nh;

    }
    close(fd);
    return WRITE_SUCCESS;
}