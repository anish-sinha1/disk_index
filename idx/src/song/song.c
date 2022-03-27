#include <cslice.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include <sys/stat.h>

// status codes
enum F_STAT {
    WRITE_SUCCESS = 1,
    WRITE_FAIL = 2,
    READ_SUCCESS = 3,
    READ_FAIL = 4,
    FMT_SUCCESS = 5,
    FMT_FAIL = 6,
    IDX_SUCCESS = 7,
    IDX_FAIL = 8,
    INSERT_SUCCESS = 9,
    INSERT_FAIL = 10
};

// paths and sizes
#define DATA_PATH "/Users/anishsinha/Home/cprojects/b+tree/data/tables/songs/data.bin"
#define IDX_PATH "/Users/anishsinha/Home/cprojects/b+tree/data/tables/indexes/song_index.bin"
#define PAGE_SIZE 8000
#define LEAF_LINK_SIZE 800

// exactly 800 bytes for convenience
struct Song {
    char title[46], artist[46], lyrics[688];
    double length;
    int likes;
};

// header of L&Y index
struct IdxHeader {
    int node_count, root_loc;
    unsigned int order;
    size_t page_size;
};

// header for each node in L&Y index. ~always~ the first 26 bytes (when _Bool is 1 byte) or 32 (when bool is just
// typedef'ed as an integer). either way, 50 bytes will be written with 24 or 18 trailing null bytes that serve as
// free space
struct NodeHeader {
    bool leaf, full;
    int rec_ct, right_link, leaf_link;
    size_t rec_size;
};

struct IdxNode {
    struct NodeHeader header;
    char *data;
};

struct ISongEntry {
    char name[46];
    int loc;
};

struct LSongEntry {
    char name[46];
    int page_loc, offset;
};

// util for generating a node header with defaults
struct NodeHeader *node_header(size_t rec_size, int order) {
    struct NodeHeader *nh = malloc(sizeof(struct NodeHeader));
    nh->full = false; // by default, a new node won't be set to full
    // by default, a new node is not a leaf since n-ary search trees such as L&Y-Trees, B+Trees, B*Trees, and BTrees
    // grow from the top
    nh->leaf = false;
    nh->rec_ct = 0; // a node is not initialized with values
    nh->rec_size = rec_size;
    // leaf_link points to a disk page which contains a continuation of a leaf node's data. by default, it does not
    // exist so the invalid -1 is the placeholder
    nh->leaf_link = -1;
    // right_link points to the disk page which fulfills the `high key` requirement of an L&Y-Tree. by default, it
    // also does not exist so the invalid -1 is the placeholder
    nh->right_link = -1;
    return nh;
}

// header for each table. the name is a touch for convenience
struct TableHeader {
    int record_count;
    size_t record_size;
    char table_name[50];
};

// util for generating table header
struct TableHeader *table_header(char *table_name, size_t record_size) {
    struct TableHeader *th = malloc(sizeof(struct TableHeader));
    th->record_count = 0;
    th->record_size = (int) record_size;
    strncpy(th->table_name, table_name, 50);
    return th;
}

struct IdxHeader *idx_header(size_t page_size, unsigned int order) {
    if (page_size < 0) return NULL;
    struct IdxHeader *ih = malloc(sizeof(struct IdxHeader));
    ih->node_count = 0;
    ih->page_size = page_size;
    ih->root_loc = -1;
    ih->order = order;
    return ih;
}

enum F_STAT fmt_idx() {
    truncate(IDX_PATH, 0);
    int fd = open(IDX_PATH, O_RDWR);
    struct IdxHeader header;
    header.root_loc = 0;
    header.node_count = 0;
    header.order = 7950 / sizeof(struct ISongEntry);
    header.page_size = PAGE_SIZE;

    // struct NodeHeader root_header;
    // root_header.rec_size = sizeof(struct LSongEntry);
    // root_header.rec_ct = 0;
    // root_header.full = false;
    // root_header.leaf = true;
    // root_header.leaf_link = root_header.right_link = -1;
    {
        // write index header
        pwrite(fd, &header, 50, 0);
        // pwrite(fd, &root_header, sizeof(struct NodeHeader), 50);
    }
    close(fd);
    return FMT_SUCCESS;
}

enum F_STAT read_table_header(struct TableHeader *th) {
    if (!th) return READ_FAIL;
    int fd = open(DATA_PATH, O_RDONLY);
    {
        pread(fd, th, sizeof(struct TableHeader), 0);
    }
    close(fd);
    return READ_SUCCESS;
}

void clear_data() {
    truncate(DATA_PATH, 0);
}

enum F_STAT fmt_table(char *table_name) {
    struct TableHeader *th = table_header("songs", sizeof(struct Song));
    strncpy(th->table_name, table_name, 50);
    th->record_count = 0;
    th->record_size = sizeof(struct Song);
    int fd = open(DATA_PATH, O_RDWR);
    if (fd < 0) return FMT_FAIL;
    // reformatting a table will destroy any data currently in it
    clear_data();
    {
        pwrite(fd, th, sizeof(struct TableHeader), 0);
    }
    close(fd);
    return FMT_SUCCESS;
}

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
    struct TableHeader song_table_header;
    if (read_table_header(&song_table_header) != READ_SUCCESS) return WRITE_FAIL;
    size_t offset = song_table_header.record_count * song_table_header.record_size + sizeof(struct TableHeader);
    {
        song_table_header.record_count++;
        pwrite(fd, &song_table_header, sizeof(struct TableHeader), 0);
        pwrite(fd, s, sizeof(struct Song), (off_t) offset);
    }
    close(fd);
    return WRITE_SUCCESS;
}

// read a song from a binary data file
enum F_STAT read_song(int pos, struct Song *s) {
    if (pos < 0) return READ_FAIL;
    int fd = open(DATA_PATH, O_RDONLY);
    struct TableHeader song_table_header;
    if (read_table_header(&song_table_header) != READ_SUCCESS) return READ_FAIL;
    if (pos > song_table_header.record_count) return READ_FAIL;
    size_t offset = song_table_header.record_size * pos + sizeof(struct TableHeader);
    pread(fd, s, sizeof(struct Song), (off_t) offset);
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
    struct TableHeader th;
    read_table_header(&th);
    if (th.record_count == 0) return slice(1);
    Slice *memory_block = slice(10);
    {
        int lim = (int) (th.record_count - PAGE_SIZE / sizeof(struct Song) * page);
        for (int i = 0; i < lim; i++) {
            struct Song *s = malloc(sizeof(struct Song));
            size_t offset = sizeof(struct Song) * i + sizeof(struct TableHeader) + PAGE_SIZE * page;
            pread(fd, s, sizeof(struct Song), (off_t) offset);
            push(memory_block, s);
        }
    }
    close(fd);
    return memory_block;
}

enum F_STAT read_idx_header(struct IdxHeader *ih) {
    int fd = open(IDX_PATH, O_RDONLY);
    if (fd < 0 || !ih) return READ_FAIL;
    {
        pread(fd, ih, sizeof(struct IdxHeader), 0);
    }
    close(fd);
    return READ_SUCCESS;
}

enum F_STAT insert_non_full(struct Song *s) {

}

enum F_STAT insert_song(struct Song *s) {
    if (!s) return INSERT_FAIL;
    int fd = open(IDX_PATH, O_RDWR);
    struct IdxHeader ih;
    if (read_idx_header(&ih) != READ_SUCCESS) return INSERT_FAIL;
    struct NodeHeader node_header;
    struct IdxNode node;
    {

    }
    close(fd);
    return INSERT_SUCCESS;
}

// generate index by song name
enum F_STAT gen_idx() {
    fmt_idx();
    int fd = open(IDX_PATH, O_RDWR);
    struct IdxHeader index_header;
    {
        // read index header
        pread(fd, &index_header, sizeof(struct NodeHeader), 0);
        // if (index_header.node_count != 0) truncate(IDX_PATH, 50);
        // // write root
        // struct NodeHeader root_header;
        // root_header.leaf_link = root_header.right_link = -1;
        // root_header.rec_ct = 0;
        // root_header.rec_size = sizeof(struct LSongEntry);
        // root_header.leaf = true;
        // root_header.full = false;
        // pwrite(fd, &root_header, sizeof(struct NodeHeader), 50);
    }
    close(fd);
    return IDX_SUCCESS;
}


void clear_idx() {
    truncate(IDX_PATH, 0);
}