#include "map.h"
#include "unity.h"

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
    uintptr_t res, status;

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
    return UNITY_END();
}


