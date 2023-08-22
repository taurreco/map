#ifndef MAP_H
#define MAP_H

/*************
 * map_entry *
 *************/

struct map_entry {
    char* key;
    int val;
}

/***********
 * hashmap *
 ***********/

struct hashmap {
    struct map_entry** entries;
    int cap;
    int len;
}

/* constructor / destructors */

struct hashmap* map_new();
void map_free();

/* insertion */

void map_put();
void map_set();

/* deletion */

void map_del();

/* retreival */

void map_get();

#endif    /* MAP_H */
