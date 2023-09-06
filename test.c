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
    
    map = map_alloc(10);
    map_free(map);
}

/*************
 * basic_put *
 ************/

void
basic_put()
{
    struct hashmap* map;
    int one;

    map = map_alloc(10);
    one = 1;

    map_put(map, "brian", &one);
    map_put(map, "dennis", &one);
    map_put(map, "alfred", &one);
    map_put(map, "jeffrey", &one);

    map_get(map, "brian", 0);
    map_get(map, "harold", 0);

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
    return UNITY_END();
}


