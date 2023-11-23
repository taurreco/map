#include "map.c"
#include "unity.h"

/*********************************************************************
 *                                                                   *
 *                        example value types                        *
 *                                                                   *
 *********************************************************************/

struct tree {
    struct tree* left;
    struct tree* right;
    int val;
};

/**************
 * tree_alloc *
 **************/

struct tree*
tree_alloc(int val, struct tree* left, struct tree* right)
{
    struct tree* tree;

    tree = malloc(sizeof(struct tree));
    tree->left = left;
    tree->right = right;
    tree->val = val;
}

/*************
 * tree_free *
 *************/

void
tree_free(struct tree* tree)
{
    if (tree->left)
        tree_free(tree->left);
    
    if (tree->right)
        tree_free(tree->right);

    free(tree);
}

/***********
 * tree_eq *
 ***********/

int
tree_eq(struct tree* a, struct tree* b)
{
    int left, right;

    left = 0;
    right = 0;

    if (a->left && b->left)
        left = tree_eq(a->left, b->left);

    if (a->left == 0 && b->left == 0)
        left = 1;

    if (a->right && b->right)
        right = tree_eq(a->right, b->right);

    if (a->right == 0 && b->right == 0)
        right = 1;

    return left && right && a->val == b->val;
}


/*********************************************************************
 *                                                                   *
 *                               misc                                *
 *                                                                   *
 *********************************************************************/

/**************
 * map_put_at *
 **************/

void
map_put_at(struct hashmap* map, char* key, uintptr_t val, int idx) 
{
    struct entry *new, *old;

    new = entry_alloc(key, val);
    
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
        
    map->entries[idx] = new;
    map->len++;
}

/**************
 * map_del_at *
 **************/

int
map_del_at(struct hashmap* map, int idx)
{
    struct entry* entry;

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

        if (entry == 0)
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
    
    return 1;    
}

/*********************************************************************
 *                                                                   *
 *                          unity helpers                            *
 *                                                                   *
 *********************************************************************/

void 
setUp() 
{
    /* empty */
}

void 
tearDown() 
{
    /* empty */
}

/*********************************************************************
 *                                                                   *
 *                           basic tests                             *
 *                                                                   *
 *********************************************************************/

/*********
 * basic *
 *********/

void
basic()
{
    struct hashmap* map;
    
    map = map_alloc(10, 0);
    map_free(map);
}

/*************
 * basic_put *
 *************/

void
basic_put()
{
    struct hashmap* map;
    uintptr_t res;
    int status;

    map = map_alloc(10, 0);

    map_put(map, "brian", 1);
    map_put(map, "dennis", 2);
    map_put(map, "alfred", 3);
    map_put(map, "jeffrey", 4);

    status = map_get(map, "brian", &res);
    TEST_ASSERT_EQUAL_INT((int)res, 1);
    TEST_ASSERT_EQUAL_INT(status, 1);

    status = map_get(map, "alfred", &res);
    TEST_ASSERT_EQUAL_INT((int)res, 3);
    TEST_ASSERT_EQUAL_INT(status, 1);

    status = map_get(map, "harold", &res);
    TEST_ASSERT_EQUAL_INT(status, 0);

    map_free(map);
}

/***************
 * basic_probe *
 ***************/

void
basic_probe()
{
    struct hashmap* map;
    uintptr_t res;
    int status;

    map = map_alloc(5, 0);

    map_put_at(map, "brian", 1, 0);
    map_put_at(map, "dennis", 2, 1);
    map_put_at(map, "alfred", 3, 2);

    /***************************************
     *                                     *
     *      |-----------|-----|-----|      *
     *      |    key    | val | psl |      *     
     *      |-----------|-----|-----|      *
     *      |  "brian"  |  1  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |  "dennis" |  2  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |  "alfred" |  3  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |     -     |  -  |  -  |      *
     *      |-----------|-----|-----|      *
     *      |     -     |  -  |  -  |      *
     *      |-----------|-----|-----|      *     
     *                                     *     
     *                                     *
     ***************************************/
    
    TEST_ASSERT_EQUAL_STRING("brian", map->entries[0]->key);
    TEST_ASSERT_EQUAL_INT(1, map->entries[0]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[0]->psl);

    TEST_ASSERT_EQUAL_STRING("dennis", map->entries[1]->key);
    TEST_ASSERT_EQUAL_INT(2, map->entries[1]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[1]->psl);

    TEST_ASSERT_EQUAL_STRING("alfred", map->entries[2]->key);
    TEST_ASSERT_EQUAL_INT(3, map->entries[2]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[2]->psl);

    TEST_ASSERT_EQUAL_INT(0, map->cost);

    map_put_at(map, "harold", 4, 0);

    /***************************************
     *                                     *
     *      |-----------|-----|-----|      *
     *      |    key    | val | psl |      *     
     *      |-----------|-----|-----|      *
     *      |  "brian"  |  1  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |  "harold" |  4  |  1  |      *
     *      |-----------|-----|-----|      *
     *      |  "dennis" |  2  |  1  |      *
     *      |-----------|-----|-----|      *
     *      |  "alfred" |  3  |  1  |      *
     *      |-----------|-----|-----|      *
     *      |     -     |  -  |  -  |      *
     *      |-----------|-----|-----|      *     
     *                                     *     
     *                                     *
     ***************************************/
    
    TEST_ASSERT_EQUAL_STRING("brian", map->entries[0]->key);
    TEST_ASSERT_EQUAL_INT(1, map->entries[0]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[0]->psl);

    TEST_ASSERT_EQUAL_STRING("harold", map->entries[1]->key);
    TEST_ASSERT_EQUAL_INT(4, map->entries[1]->val);
    TEST_ASSERT_EQUAL_INT(1, map->entries[1]->psl);

    TEST_ASSERT_EQUAL_STRING("dennis", map->entries[2]->key);
    TEST_ASSERT_EQUAL_INT(2, map->entries[2]->val);
    TEST_ASSERT_EQUAL_INT(1, map->entries[2]->psl);

    TEST_ASSERT_EQUAL_STRING("alfred", map->entries[3]->key);
    TEST_ASSERT_EQUAL_INT(3, map->entries[3]->val);
    TEST_ASSERT_EQUAL_INT(1, map->entries[3]->psl);

    TEST_ASSERT_EQUAL_INT(3, map->cost);

    map_free(map);
}


/*************
 * basic_del *
 *************/

void
basic_del()
{
    struct hashmap* map;
    int status;

    map = map_alloc(5, 0);

    map_put_at(map, "brian", 1, 0);
    map_put_at(map, "dennis", 2, 1);
    map_put_at(map, "alfred", 3, 2);
    map_put_at(map, "harold", 4, 0);

    /***************************************
     *                                     *
     *      |-----------|-----|-----|      *
     *      |    key    | val | psl |      *     
     *      |-----------|-----|-----|      *
     *      |  "brian"  |  1  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |  "harold" |  4  |  1  |      *
     *      |-----------|-----|-----|      *
     *      |  "dennis" |  2  |  1  |      *
     *      |-----------|-----|-----|      *
     *      |  "alfred" |  3  |  1  |      *
     *      |-----------|-----|-----|      *
     *      |     -     |  -  |  -  |      *
     *      |-----------|-----|-----|      *     
     *                                     *     
     *                                     *
     ***************************************/

    status = map_del_at(map, 1);    /* delete harold */

    /***************************************
     *                                     *
     *      |-----------|-----|-----|      *
     *      |    key    | val | psl |      *     
     *      |-----------|-----|-----|      *
     *      |  "brian"  |  1  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |  "dennis" |  2  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |  "alfred" |  3  |  0  |      *
     *      |-----------|-----|-----|      *
     *      |     -     |  -  |  -  |      *
     *      |-----------|-----|-----|      *
     *      |     -     |  -  |  -  |      *
     *      |-----------|-----|-----|      *     
     *                                     *     
     *                                     *
     ***************************************/

    TEST_ASSERT_EQUAL_INT(1, status);
    TEST_ASSERT_EQUAL_STRING("brian", map->entries[0]->key);
    TEST_ASSERT_EQUAL_INT(1, map->entries[0]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[0]->psl);

    TEST_ASSERT_EQUAL_STRING("dennis", map->entries[1]->key);
    TEST_ASSERT_EQUAL_INT(2, map->entries[1]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[1]->psl);

    TEST_ASSERT_EQUAL_STRING("alfred", map->entries[2]->key);
    TEST_ASSERT_EQUAL_INT(3, map->entries[2]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[2]->psl);

    TEST_ASSERT_EQUAL_INT(0, map->cost);

    map_free(map);
 }

/**************
 * basic_grow *
 **************/

void
basic_grow()
{
    struct hashmap* map;

    map = map_alloc(5, 0);

    map_put_at(map, "brian", 1, 0);
    map_put_at(map, "dennis", 2, 1);
    map_put_at(map, "alfred", 3, 2);

    grow(map);

    TEST_ASSERT_EQUAL_INT(11, map->cap);
    TEST_ASSERT_EQUAL_INT(3, map->len);

    TEST_ASSERT_EQUAL_STRING("brian", map->entries[0]->key);
    TEST_ASSERT_EQUAL_INT(1, map->entries[0]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[0]->psl);

    TEST_ASSERT_EQUAL_STRING("dennis", map->entries[1]->key);
    TEST_ASSERT_EQUAL_INT(2, map->entries[1]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[1]->psl);

    TEST_ASSERT_EQUAL_STRING("alfred", map->entries[2]->key);
    TEST_ASSERT_EQUAL_INT(3, map->entries[2]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[2]->psl);

    TEST_ASSERT_EQUAL_PTR(0, map->entries[3]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[4]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[5]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[6]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[7]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[8]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[9]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[10]);

    TEST_ASSERT_EQUAL_INT(0, map->cost);

    map_free(map);
}

/****************
 * basic_shrink *
 ****************/

void
basic_shrink()
{
    struct hashmap* map;

    map = map_alloc(13, 0);

    map_put_at(map, "brian", 1, 0);
    map_put_at(map, "dennis", 2, 1);

    shrink(map);

    TEST_ASSERT_EQUAL_INT(7, map->cap);
    TEST_ASSERT_EQUAL_INT(2, map->len);

    TEST_ASSERT_EQUAL_STRING("brian", map->entries[0]->key);
    TEST_ASSERT_EQUAL_INT(1, map->entries[0]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[0]->psl);

    TEST_ASSERT_EQUAL_STRING("dennis", map->entries[1]->key);
    TEST_ASSERT_EQUAL_INT(2, map->entries[1]->val);
    TEST_ASSERT_EQUAL_INT(0, map->entries[1]->psl);

    TEST_ASSERT_EQUAL_PTR(0, map->entries[2]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[3]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[4]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[5]);
    TEST_ASSERT_EQUAL_PTR(0, map->entries[6]);

    TEST_ASSERT_EQUAL_INT(0, map->cost);

    map_free(map);
}


/*********************************************************************
 *                                                                   *
 *                             helpers                               *
 *                                                                   *
 *********************************************************************/

/******************
 * check_is_prime *
 ******************/

 void
 check_is_prime()
 {
    TEST_ASSERT_TRUE(is_prime(2));
    TEST_ASSERT_TRUE(is_prime(3));
    TEST_ASSERT_TRUE(is_prime(5));
    TEST_ASSERT_TRUE(is_prime(7));
    TEST_ASSERT_TRUE(is_prime(11));
    TEST_ASSERT_TRUE(is_prime(13));
    TEST_ASSERT_TRUE(is_prime(5381));

    TEST_ASSERT_FALSE(is_prime(1));
    TEST_ASSERT_FALSE(is_prime(4));
    TEST_ASSERT_FALSE(is_prime(6));
    TEST_ASSERT_FALSE(is_prime(9));
    TEST_ASSERT_FALSE(is_prime(10));
    TEST_ASSERT_FALSE(is_prime(33));
 }


/********************
 * check_next_prime *
 ********************/

 void
 check_next_prime()
 {
    TEST_ASSERT_EQUAL_INT(2, next_prime(0));
    TEST_ASSERT_EQUAL_INT(2, next_prime(1));
    TEST_ASSERT_EQUAL_INT(2, next_prime(2));
    TEST_ASSERT_EQUAL_INT(11, next_prime(9));
    TEST_ASSERT_EQUAL_INT(11, next_prime(11));
    TEST_ASSERT_EQUAL_INT(131, next_prime(130));
    TEST_ASSERT_EQUAL_INT(383, next_prime(380));
    TEST_ASSERT_EQUAL_INT(7817, next_prime(7794));
 }

/*********************************************************************
 *                                                                   *
 *                          probing tests                            *
 *                                                                   *
 *********************************************************************/

/*********************
 * check_tree_values *
 *********************/

void
check_tree_values()
{
    struct hashmap* map;
    struct tree *a, *b, *c, *d;
    uintptr_t res;
    int status;

    map = map_alloc(10, tree_free);

    a = tree_alloc(1, 
            tree_alloc(2, 0, 0), 
            tree_alloc(3, 0, tree_alloc(4, 0, 0)));
    b = tree_alloc(1,
            tree_alloc(2, 0, 0),
            tree_alloc(3, 0, 0));
    c = tree_alloc(1, 
            tree_alloc(2, 0, 0), 0);
    d = tree_alloc(1, 0, 0);

    map_put(map, "brian", a);
    map_put(map, "alfred", b);
    map_put(map, "harold", c);
    map_put(map, "jeffrey", d);

    status = map_get(map, "brian", &res);
    TEST_ASSERT_EQUAL_INT(status, 1);
    TEST_ASSERT_TRUE(tree_eq((struct tree*)res, a));

    status = map_get(map, "alfred", &res);
    TEST_ASSERT_EQUAL_INT(status, 1);
    TEST_ASSERT_TRUE(tree_eq((struct tree*)res, b));

    status = map_get(map, "harold", &res);
    TEST_ASSERT_EQUAL_INT(status, 1);
    TEST_ASSERT_TRUE(tree_eq((struct tree*)res, c));

    status = map_get(map, "jeffrey", &res);
    TEST_ASSERT_EQUAL_INT(status, 1);
    TEST_ASSERT_TRUE(tree_eq((struct tree*)res, d));

    map_free(map);
}

/*********************************************************************
 *                                                                   *
 *                              main                                 *
 *                                                                   *
 *********************************************************************/

/********
 * main *
 ********/

int
main() 
{
    UNITY_BEGIN();
    RUN_TEST(basic);
    RUN_TEST(basic_put);
    RUN_TEST(basic_probe);
    RUN_TEST(basic_del);
    RUN_TEST(basic_grow);
    RUN_TEST(basic_shrink);

    RUN_TEST(check_tree_values);
    RUN_TEST(check_is_prime);
    RUN_TEST(check_next_prime);

    return UNITY_END();
}


