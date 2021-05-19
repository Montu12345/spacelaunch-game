#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "collision.h"
#include "sprite.h"
#include "body.h"

const int RADIUS = 10;

void test_collision()
{
    list_t *star = sprite_make_star(10, 10, 20);
    body_t *b = body_init(star, 2, rgb_init_random_bright());

    // Tests the same shape overlaps
    assert(find_collision(star, star).collided);

    // Tests two far away shapes overlap
    body_set_centroid(b, (vector_t){.x = 20, .y = 20});
    list_t *body_shape = body_get_shape(b);
    assert(find_collision(body_shape, star).collided);
    assert(find_collision(star, body_shape).collided);
    list_free(body_shape);

    // Tests two circles moving away from each other
    list_t *circle = sprite_make_circle(RADIUS);
    list_t *circle2 = sprite_make_circle(RADIUS);
    vector_t translate = {.x = 1, .y = 0};
    vector_t opposite_translate = vec_multiply(-1, translate);
    for (int i = 0; i < RADIUS - 1; i++)
    {
        // Move the circles
        polygon_translate(circle, translate);
        polygon_translate(circle2, opposite_translate);

        // Assert the circles are still overlapping
        assert(find_collision(circle, circle2).collided);
        assert(find_collision(circle2, circle).collided);
    }
    polygon_translate(circle, translate);
    polygon_translate(circle2, opposite_translate);
    assert(!find_collision(circle, circle2).collided);
    assert(!find_collision(circle2, circle).collided);

    list_free(circle);
    list_free(circle2);
    body_free(b);
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

    DO_TEST(test_collision)

    puts("test_collision PASS");
}
