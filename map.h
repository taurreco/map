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
    struct map_entry** buckets;
    int cap;
    int len;
}

#endif    /* MAP_H */
