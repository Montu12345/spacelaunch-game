#include "test_util.h"
#include "sprite.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

void test_make_star()
{
    // TODO: test the shorter points as well.
    // TODO: Stress test large star
    list_t *star = sprite_make_star(3, 4, 5);
    vector_t *vec = list_get(star, 4);
    assert(vec_isclose(*vec, (vector_t){-5.0/2, -5.0/2*sqrt(3)}));
    vector_t *vec_2 = list_get(star, 2);
    assert(vec_isclose(*vec_2, (vector_t){-5.0/2, 5.0/2*sqrt(3)}));
    vector_t *vec_3 = list_get(star, 0);
    assert(vec_isclose(*vec_3, (vector_t){5, 0}));
    list_free(star);
}

void test_make_rect()
{
    list_t *rect = sprite_make_rect(0, 2, 0, 2);
    vector_t *vec = list_get(rect, 3);
    assert(vec_isclose(*vec, (vector_t){2, 0}));
    vector_t *vec_2 = list_get(rect, 0);
    assert(vec_isclose(*vec_2, (vector_t){2, 2}));
    list_free(rect);
}

int main(int argc, char *argv[])
{
    // Run all tests? True if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests)
    {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_make_star);
    DO_TEST(test_make_rect);

    puts("make_star PASS");
}

