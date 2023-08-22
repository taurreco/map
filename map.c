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
   
    map = malloc(sizeof(struct hashmap));
    map->cap = cap;
    map->len = 0;
    map->entries = calloc(cap, sizeof(struct map_entry*));
    return ht;
}

/************
 * map_free *
 ************/

void
map_free(struct map* map) 
{
    for (int i = 0; i < map->len; i++) {
        struct map_entry* entry;
	
	entry = map->entries[i];

        if (pair)
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

/**********
 * hash_1 *
 **********/

static int
hash_1(char* key)
{
    int64_t hash = 5381;
    int c;

    while ((c = *key++))
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return (int)hash;
}

/**********
 * hash_2 *
 **********/

static int
hash_2(char* key)
{
    int hash, i;
    int key_len = strlen(key);

    for (hash = i = 0; i < key_len; i++) {
        hash += key[i];
        hash += (hash << 10);
        hash ^= (hash >> 6);
    }
    hash += (hash << 3);
    hash ^= (hash >> 11);
    hash += (hash << 15);

    return hash;
}

/********
 * grow *
 ********/

void
resize(struct hashmap* map, int new_cap)
{
    SIZES_INDEX++;
    struct hash_table* new_ht = hash_table_alloc();

    for (int i = 0; i < ht->size; i++) {
        struct kv_pair* pair = ht->pairs[i];
        if (pair) insert(new_ht, pair->key, pair->val);
    }

    ht->size = new_ht->size;
    ht->n_pairs = new_ht->n_pairs;

    ht->size = SIZES[SIZES_INDEX];
    new_ht->size = SIZES[SIZES_INDEX - 1];

    /* swap list of kv_pairs */

    struct kv_pair** tmp_pairs = ht->pairs;
    ht->pairs = new_ht->pairs;
    new_ht->pairs = tmp_pairs;

    hash_table_free(new_ht);
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

