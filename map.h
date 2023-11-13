#ifndef MAP_H
#define MAP_H

#include <stdint.h>

struct hashmap;

/* constructor / destructors */

struct hashmap* map_alloc(int cap, void (*val_free)(void*));

void map_free(struct hashmap* map);

/* insertion */

void map_put(struct hashmap* map, char* key, uintptr_t val);
void map_set();

/* deletion */

int map_del();

/* retreival */

int map_get();

#endif    /* MAP_H */
