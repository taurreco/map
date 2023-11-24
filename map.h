#ifndef MAP_H
#define MAP_H

#include <stdint.h>

#define EMAPNOENTRY -30

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

/* iteration */

void map_begin(struct hashmap* map);
int map_end(struct hashmap* map);
void map_cur(struct hashmap* map, char* key, uintptr_t* val);
void map_next(struct hashmap* map);

/*
map_begin(map); 
while (!map_end(map)) {
    char key[10];
    uintptr_t val;
    map_cur(map, key, &val);
    ...
    map_next(map);
}
*/

#endif    /* MAP_H */
