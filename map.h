#ifndef MAP_H
#define MAP_H

#include <stdint.h>

struct hashmap;

/* constructor / destructors */

struct hashmap* map_alloc(int cap, void (*val_free)(void*));
void map_free(struct hashmap* map);

/* insertion */

void map_put(struct hashmap* map, char* key, uintptr_t val);
int map_set(struct hashmap* map, char* key, uintptr_t val);

/* deletion */

int map_del(struct hashmap* map, char* key);

/* retreival */

int map_get(struct hashmap* map, char* key, uintptr_t* res);

#endif    /* MAP_H */
