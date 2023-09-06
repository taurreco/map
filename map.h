#ifndef MAP_H
#define MAP_H

typedef void (*free_fn)(void* a);
typedef int (*cmp_fn)(void* a, void* b);
typedef int (*len_fn)(void* key);
typedef int (*hash_fn)(void* key);

/*************
 * map_entry *
 *************/

struct map_entry {
    union {
        long key;
	void* keyp;
    };
    union {
        long val;
	void* valp;
    };
};

/***********
 * hashmap *
 ***********/

struct hashmap {
    struct map_entry* entries;
    free_fn val_free;
    free_fn key_free;
    cmp_fn key_cmp;
    hash_fn hash;
    int cap;
    int len;
};

/* constructor / destructors */

struct hashmap* map_alloc(free_fn val_free, free_fn key_free, 
		          cmp_fn key_cmp, hash_fn hash, int cap);

void map_free(struct hashmap* map);

/* insertion */

void map_put(struct hashmap* map, void* key, void* val);
void map_set();

/* deletion */

void map_del();

/* retreival */

void map_get();

#endif    /* MAP_H */
