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

int
hash(char* key)
{
    int hash, len;
    
    hash = 0;
    len = strlen(key);

    for (int i = 0; i < len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }

    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

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
    struct entry* entry;
    entry = entry_alloc(key, val);
    
    map->entries[map->len] = entry;
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

    entry = map->entries[0];

    if (entry == 0)
        return 0;
    
    *res = entry->val;

    return 1;    
}

