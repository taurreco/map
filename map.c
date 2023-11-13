#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "map.h"


typedef void (*free_fn)(void* a);

/*********
 * entry *
 *********/

struct entry {
    char* key;
    uintptr_t val;    /* can also be a int, long, etc < 8 bytes */
    int psl;
};

/***********
 * hashmap *
 ***********/

struct hashmap {
    struct entry** entries;
    free_fn val_free;
    int cap;
    int len;
    int cost;
    int maxpsl;
};


/*********************************************************************
 *                                                                   *
 *                      constructor / destructor                     *
 *                                                                   *
 *********************************************************************/

/***************
 * entry_alloc *
 ***************/

struct entry* 
entry_alloc(char* key, uintptr_t val)
{
    struct entry* entry;
    
    entry = malloc(sizeof(struct entry));
    entry->key = strdup(key);
    entry->val = val;
    entry->psl = 0;
    return entry;
}

/**************
 * entry_free *
 **************/

void
entry_free(struct entry* entry, free_fn val_free) 
{
    free(entry->key);
    if (val_free)
        val_free((void*)entry->val);
    free(entry);
}

/*************
 * map_alloc *
 *************/

struct hashmap*
map_alloc(int cap, free_fn val_free)
{
    struct hashmap* map;
    struct entry** entries;
   
    map = malloc(sizeof(struct hashmap));
    entries = calloc(cap, sizeof(struct entry*));
    map->cap = cap;
    map->len = 0;
    map->cost = 0;
    map->maxpsl = 0;
    map->entries = entries;
    map->val_free = val_free;
    return map;
}

/************
 * map_free *
 ************/

void
map_free(struct hashmap* map) 
{
    for (int i = 0; i < map->cap; i++) {
        struct entry* entry;
	
	    entry = map->entries[i];

        if (entry)
            entry_free(entry, map->val_free);
    }

    free(map->entries);
    free(map);
}

/*********************************************************************
 *                                                                   *
 *                              utility                              *
 *                                                                   *
 *********************************************************************/

/*******
 * max *
 *******/

static int
max(int a, int b)
{
    return a > b ? a : b;
}

/********
 * hash *
 ********/

 /* djb2 http://www.cse.yorku.ca/~oz/hash.html */

static uint64_t
hash(char* str)
{
    uint64_t hash;
    int c;

    hash = 5381;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/**********
 * resize *
 **********/

static void
resize(struct hashmap* map, int new_cap)
{
}

/*********************************************************************
 *                                                                   *
 *                             insertion                             *
 *                                                                   *
 *********************************************************************/

/***********
 * map_set *
 ***********/

void
map_set(struct hashmap* map, char* key, int val) 
{
}

/***********
 * map_put *
 ***********/

 /* table insertion with robin hood probing */

void
map_put(struct hashmap* map, char* key, uintptr_t val) 
{
    struct entry *new, *old;
    int idx;

    new = entry_alloc(key, val);
    idx = hash(key) % map->cap;
    
    /* probe routine */
    while (1) {
        
        old = map->entries[idx];

        /* empty slot */
        if (old == 0) {
            break;        
        }

        /* swap */
        if (old->psl < new->psl) {
            map->entries[idx] = new;
            new = old;
        }

        idx++;
        new->psl++;
        map->cost++;
    }

    map->maxpsl = max(map->maxpsl, new->psl);
    map->entries[idx] = new;
    map->len++;
}

/*********************************************************************
 *                                                                   *
 *                             deletion                              *
 *                                                                   *
 *********************************************************************/

/***********
 * map_del *
 ***********/

int
map_del(struct hashmap* map, char* key)
{

}


/*********************************************************************
 *                                                                   *
 *                             retrieval                             *
 *                                                                   *
 *********************************************************************/

/***********
 * map_get *
 ***********/

/* smart-search heuristic table lookup */

int
map_get(struct hashmap* map, char* key, uintptr_t* res)
{
    struct entry* entry;
    int idx, mean, up, down;

    mean = map->cost / map->len;
    down = mean + 1;
    up = mean;

    idx = hash(key) % map->cap;

    while (1) {
        
        /* fail if we exceed boundary */
        if (idx + up < 0 || idx + down >= map->cap)
            return 0;
        
        /* fail if we exceed maxpsl */
        if (down - up > 2 * map->maxpsl) {
            return 0;
        }

        entry = map->entries[idx + up];
        if (entry && strcmp(entry->key, key) == 0)
            break;

        entry = map->entries[idx + down];
        if (entry && strcmp(entry->key, key) == 0)
            break;

        down++;
        up--;
    }
    
    *res = entry->val;
    return 1;    
}

