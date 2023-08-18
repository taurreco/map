#ifndef MAP_H
#define MAP_H

/***********
 * hashmap *
 ***********/

struct hashmap {
    
}

struct kv_pair {
    char* key;
    int val;
};

struct hash_table {
    struct kv_pair** pairs;         /* list of obj indices */
    int n_pairs;                    /* running count of keys */
    int size;                       /* number of buckets in ht */
};


#endif    /* MAP_H */
