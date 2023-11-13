#include "map.c"
#include "unity.h"

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
    }

        
    map->entries[idx] = new;
    map->len++;
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
    
    TEST_ASSERT_EQUAL_STRING(map->entries[0]->key, "brian");
    TEST_ASSERT_EQUAL_INT(map->entries[0]->val, 1);
    TEST_ASSERT_EQUAL_INT(map->entries[0]->psl, 0);

    TEST_ASSERT_EQUAL_STRING(map->entries[1]->key, "dennis");
    TEST_ASSERT_EQUAL_INT(map->entries[1]->val, 2);
    TEST_ASSERT_EQUAL_INT(map->entries[1]->psl, 0);

    TEST_ASSERT_EQUAL_STRING(map->entries[2]->key, "alfred");
    TEST_ASSERT_EQUAL_INT(map->entries[2]->val, 3);
    TEST_ASSERT_EQUAL_INT(map->entries[2]->psl, 0);

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
    return UNITY_END();
}


