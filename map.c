#include <string.h>
#include <stdint.h>
#include <stdlib.h>

#include "map.h"

#define MAX_PRIME_GAP 222    /* maximum prime gap for numbers under 100 mil */
#define HASH_BASE 5381

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
    void (*val_free)(void*);
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

static struct entry* 
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

static void
entry_free(struct entry* entry, void (*val_free)(void*)) 
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
map_alloc(int cap, void (*val_free)(void*))
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
min(int a, int b)
{
    return a < b ? a : b;
}

/*******
 * max *
 *******/

static int
max(int a, int b)
{
    return a > b ? a : b;
}

/************
 * is_prime *
 ************/

/* 1 if prime, 0 if not */

static int
is_prime(int n)
{
    if (n <= 1)
        return 0;
    
    if (n == 2 || n == 3)
        return 1;
    
    if (n % 2 == 0 || n % 3 == 0)
        return 0;
    
    for (int i = 5; i * i <= n; i = i + 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
 
    return 1;
}

/*********
 * prime *
 *********/

/* finds closest prime to n greater than n */

static int
prime(int n)
{
    int i;

    i = 0;
    while (!is_prime(n)) {
        if (i > MAX_PRIME_GAP)
            break;
        n++;
        i++;
    }

    return n;
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

    hash = HASH_BASE;

    while (c = *str++)
        hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}

/********
 * find *
 ********/

/* returns index into map the key value pair lives, or -1 if not found */

static int
find(struct hashmap* map, char* key)
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
            return -1;
        
        /* fail if we exceed maxpsl */
        if (down - up > 2 * map->maxpsl) {
            return -1;
        }

        entry = map->entries[idx + up];
        if (entry && strcmp(entry->key, key) == 0)
            return idx + up;

        entry = map->entries[idx + down];
        if (entry && strcmp(entry->key, key) == 0)
            return idx + down;

        down++;
        up--;
    }
}


/**********
 * resize *
 **********/

static void
resize(struct hashmap* map, int new_cap)
{
    struct hashmap* new;
    struct entry** tmp;

    new_cap = prime(new_cap);
    
    new = map_alloc(new_cap, map->val_free);
    for (int i = 0; i < min(map->cap, new_cap); i++) {
        new->entries[i] = map->entries[i];
        map->entries[i] = 0;
    }

    tmp = map->entries;

    map->cap = new_cap;
    map->entries = new->entries;

    new->entries = tmp;
    map_free(new);
}

/********
 * grow *
 ********/

static void
grow(struct hashmap* map)
{
    if (map->len >= 3 * map->cap / 4) {
        resize(map, 2 * map->cap);
    }
}

/**********
 * shrink *
 **********/

static void
shrink(struct hashmap* map)
{
    if (map->len <= map->cap / 4) {
        resize(map, map->cap / 2);
    }
}

/*********************************************************************
 *                                                                   *
 *                             insertion                             *
 *                                                                   *
 *********************************************************************/

/***********
 * map_set *
 ***********/

int
map_set(struct hashmap* map, char* key, uintptr_t val) 
{
    struct entry *new, *old;
    int idx;

    idx = find(map, key);

    /* key not in map */
    if (idx == -1)
        return 0;

    new = entry_alloc(key, val);
    old = map->entries[idx];

    map->entries[idx] = new;
    entry_free(old, map->val_free);
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

    grow(map);
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
    struct entry* entry;
    int idx;

    idx = find(map, key);

    /* key not in map */
    if (idx == -1)
        return 0;

    /* remove key from map */

    entry = map->entries[idx];
    map->entries[idx] = 0;
    map->cost -= entry->psl;

    entry_free(entry, map->val_free);

    map->len--;
    idx++;   

    /* back-shift routine */

    while (1) {
        entry = map->entries[idx];
        
        /* not sure what to do here*/
        if (idx >= map->cap)
            break;
        
        /* leave my brother */
        if (entry->psl <= 0)
            break;

        entry->psl--;
        map->cost--;
        map->entries[idx - 1] = entry;
        map->entries[idx] = 0;
        idx++;
    }

    shrink(map);
    
    return 1;    
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
    int idx;

    idx = find(map, key);

    if (idx == -1)
        return 0;

    entry = map->entries[idx];
    *res = entry->val;

    return 1;    
}

