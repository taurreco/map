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

/********
 * hash *
 ********/

 /* djb2 http://www.cse.yorku.ca/~oz/hash.html */

uint64_t
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

void
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
    }

        
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

int
map_get(struct hashmap* map, char* key, uintptr_t* res)
{
    struct entry* entry;
    int idx;

    idx = hash(key) % map->cap;
    entry = map->entries[idx];

    if (entry == 0)
        return 0;
    
    *res = entry->val;

    return 1;    
}

