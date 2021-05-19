#include "vector.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>

void test_vec_zero()
{
    assert(VEC_ZERO.x == 0.0);
    assert(VEC_ZERO.y == 0.0);
}

void test_vec_add()
{
    assert(vec_equal(vec_add(VEC_ZERO, VEC_ZERO), VEC_ZERO));
    assert(vec_equal(vec_add(VEC_ZERO, (vector_t){1, 2}), (vector_t){1, 2}));
    assert(vec_equal(vec_add((vector_t){1, 5}, (vector_t){2, 10}), (vector_t){3, 15}));
    assert(vec_equal(vec_add((vector_t){-1.5, -1.5}, (vector_t){2.5, 2.5}), (vector_t){1, 1}));
}

void test_vec_subtract()
{
    assert(vec_equal(vec_subtract(VEC_ZERO, VEC_ZERO), VEC_ZERO));
    assert(vec_equal(vec_subtract(VEC_ZERO, (vector_t){1, 2}), (vector_t){-1, -2}));
    assert(vec_equal(vec_subtract((vector_t){3, 15}, (vector_t){2, 10}), (vector_t){1, 5}));
    assert(vec_equal(vec_subtract((vector_t){1.5, 1.5}, (vector_t){-2.5, -2.5}), (vector_t){4, 4}));
    assert(vec_equal(vec_subtract((vector_t){-1, -2}, (vector_t){3, 4}), (vector_t){-4, -6}));
}

void test_vec_project(){
    vector_t v1 = {.x = 1, .y = 1};
    vector_t v2 = VEC_ZERO;
    assert(vec_isclose(vec_project(v1, v2), VEC_ZERO));
}

void test_vec_project_to_line(){
    vector_t p = {.x = 1, .y = 1};
    vector_t l1 = {.x = 1, .y = 1};
    vector_t l2 = {.x = 2, .y = 2};
    assert(vec_isclose(vec_project_to_line(p, l1, l2), VEC_ZERO));
}

void test_vec_negate()
{
    assert(vec_equal(vec_negate(VEC_ZERO), VEC_ZERO));
    assert(vec_equal(vec_negate((vector_t){-5, 6}), (vector_t){5, -6}));
    assert(vec_equal(vec_negate((vector_t){2, -3}), (vector_t){-2, 3}));
}

void test_vec_multiply()
{
    assert(vec_equal(vec_multiply(0, (vector_t){5, 5}), VEC_ZERO));
    assert(vec_equal(vec_multiply(1, (vector_t){5, 7}), (vector_t){5, 7}));
    assert(vec_equal(vec_multiply(1, (vector_t){5, 7}), (vector_t){5, 7}));
    assert(vec_equal(vec_multiply(10, (vector_t){2, 3}), (vector_t){20, 30}));
    assert(vec_equal(vec_multiply(10, (vector_t){-2, -3}), (vector_t){-20, -30}));
    assert(vec_equal(vec_multiply(-3, (vector_t){7, 5}), (vector_t){-21, -15}));
    assert(vec_equal(vec_multiply(0.5, (vector_t){-2, 3}), (vector_t){-1, 1.5}));
}

void test_vec_dot()
{
    assert(vec_dot(VEC_ZERO, (vector_t){1, 2}) == 0);
    assert(vec_dot((vector_t){1, 2}, (vector_t){3, 4}) == 11);
    assert(vec_dot((vector_t){-5, 3}, (vector_t){2, 7}) == 11);
}

void test_vec_cross()
{
    assert(vec_cross(VEC_ZERO, (vector_t){1, 2}) == 0);
    assert(vec_cross((vector_t){1, 2}, (vector_t){3, 4}) == -2);
    assert(vec_cross((vector_t){-5, 3}, (vector_t){2, 7}) == -41);
}

void test_vec_rotate()
{
    // No rotation
    assert(vec_isclose(vec_rotate((vector_t){5, 7}, 0), (vector_t){5, 7}));
    // 90-degree rotation
    assert(vec_isclose(vec_rotate((vector_t){5, 7}, 0.5 * M_PI), (vector_t){-7, 5}));
    // 180-degree rotation
    assert(vec_isclose(vec_rotate((vector_t){5, 7}, M_PI), (vector_t){-5, -7}));
    // 270-degree rotation
    assert(vec_isclose(vec_rotate((vector_t){5, 7}, 1.5 * M_PI), (vector_t){7, -5}));
    // 3-4-5 triangle
    assert(vec_isclose(vec_rotate((vector_t){5, 0}, acos(4.0 / 5.0)), (vector_t){4, 3}));
    // Rotate (0, 0)
    assert(vec_isclose(vec_rotate(VEC_ZERO, 1.0), VEC_ZERO));
}

void test_vec_angle_between()
{
    // 0 degrees
    assert(vec_angle_between((vector_t){3, 4}, (vector_t){3, 4}) == 0.0);
    // pi/2 degrees
    assert(vec_angle_between((vector_t){3, 4}, (vector_t){-4, 3}) == M_PI / 2);
    // pi degrees
    assert(vec_angle_between((vector_t){3, 4}, (vector_t){-3, -4}) == M_PI);
}

// need help
void test_vec_reflect_across_line()
{
    // line = {3, 4}
    vector_t reflected_vec_a = {.x = 3, .y = 4};
    assert(vec_equal(vec_reflect_across_line((vector_t){3, 4}, (vector_t){3, 4}), reflected_vec_a));
    // line = {3, 5}
    vector_t reflected_vec_b = {.x = 36 / 17, .y = 77 / 17};
    assert(vec_equal(vec_reflect_across_line((vector_t){3, 4}, (vector_t){3, 5}), reflected_vec_b));
    // line = {5, 10}
    vector_t reflected_vec_c = {.x = 7 / 5, .y = 24 / 5};
    assert(vec_equal(vec_reflect_across_line((vector_t){3, 4}, (vector_t){5, 10}), reflected_vec_c));
    // line = {7, 2}
    vector_t reflected_vec_d = {.x = 247 / 53, .y = 368 / 53};
    assert(vec_equal(vec_reflect_across_line((vector_t){3, 4}, (vector_t){7, 2}), reflected_vec_d));

    // int vec = vec_reflect_across_line((vector_t){3, 4}, (vector_t){3,4});

    //wtf
    assert(vec_equal(vec_reflect_across_line((vector_t){3, 4}, (vector_t){3, 4}), (vector_t){3, 4}));
    assert(vec_equal(vec_multiply(0, (vector_t){5, 5}), VEC_ZERO));
}

int main(int argc, char *argv[])
{
    // Run all tests if there are no command-line arguments
    bool all_tests = argc == 1;
    // Read test name from file
    char testname[100];
    if (!all_tests)
    {
        read_testname(argv[1], testname, sizeof(testname));
    }

    DO_TEST(test_vec_zero)
    DO_TEST(test_vec_add)
    DO_TEST(test_vec_subtract)
    DO_TEST(test_vec_negate)
    DO_TEST(test_vec_multiply)
    DO_TEST(test_vec_dot)
    DO_TEST(test_vec_cross)
    DO_TEST(test_vec_rotate)
    DO_TEST(test_vec_angle_between)
    DO_TEST(test_vec_project)
    DO_TEST(test_vec_project_to_line)

    puts("vector_test PASS");
}
