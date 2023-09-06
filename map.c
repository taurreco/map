#include <string.h>
#include <stdlib.h>

#include "map.h"

/*********************************************************************
 *                                                                   *
 *                      constructor / destructor                     *
 *                                                                   *
 *********************************************************************/

/***************
 * entry_alloc *
 ***************/

struct map_entry* 
entry_alloc(char* key, int val)
{
    struct map_entry* entry;
    
    entry = malloc(sizeof(struct map_entry));
    entry->key = strdup(key);
    entry->val = val;
    return entry;
}

/**************
 * entry_free *
 **************/

void
entry_free(struct map_entry* entry) 
{
    free(entry->key);
    free(entry);
}

/*************
 * map_alloc *
 *************/

struct hashmap*
map_alloc(int cap)
{
    struct hashmap* map;
    struct map_entry** entries;
   
    map = malloc(sizeof(struct hashmap));
    entries = calloc(cap, sizeof(struct map_entry*));
    map->cap = cap;
    map->len = 0;
    map->entries = entries;
    return map;
}

/************
 * map_free *
 ************/

void
map_free(struct hashmap* map) 
{
    for (int i = 0; i < map->len; i++) {
        struct map_entry* entry;
	
	entry = map->entries[i];

        if (entry)
            entry_free(entry);
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
map_put(struct hashmap* map, char* key, int val) 
{
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

map_get(struct hashmap* map, char* key)
{
}

