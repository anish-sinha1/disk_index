#ifndef B_TREE_COMMON_H
#define B_TREE_COMMON_H

#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <stdbool.h>

#define DATA_PATH "/Users/anishsinha/Home/cprojects/b+tree/data/tables/songs/data.bin"
#define IDX_PATH "/Users/anishsinha/Home/cprojects/b+tree/data/indexes/songs/song_index.bin"
#define PAGE_SIZE 8000

// exactly 800 bytes for convenience
struct Song {
    char title[46], artist[46], lyrics[688];
    double length;
    int likes;
};

struct IdxHeader {
    int node_count, page_size, root_loc;
};

struct IdxHeader *idx_header(int nodes, int page_size, int root_loc) {
    if (page_size < 0) return NULL;
    struct IdxHeader *ih = malloc(sizeof(struct IdxHeader));
    ih->node_count = nodes;
    ih->page_size = page_size;
    ih->root_loc = root_loc;
    return ih;
}

struct NodeHeader {
    int leaf, num_entries, link;
};

struct NodeHeader *node_header(int leaf, int num_entries, int link) {
    struct NodeHeader *nh = malloc(sizeof(struct NodeHeader));
    nh->leaf = leaf;
    nh->num_entries = num_entries;
    nh->link = link;
    return nh;
}

enum F_STAT {
    WRITE_SUCCESS = 1, WRITE_FAIL = 2, READ_SUCCESS = 3, READ_FAIL = 4, FMT_SUCCESS = 5, FMT_FAIL = 6
};

#endif