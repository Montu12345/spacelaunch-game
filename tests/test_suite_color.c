#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include "color.h"

void test_init_rgb_from_hsv()
{
    rgb_color_t color_2 = rgb_from_hsv_init(30, 0.5, 0.6);
    assert(color_2.r * 255 - 153 < 0.1);
    assert(color_2.b * 255 - 76.5 < 0.1);
    rgb_color_t color_4 = rgb_from_hsv_init(350, 0.9, 0.3);
    assert(color_4.r * 255 - 76.5 < 0.1);
    assert(color_4.g * 255 - 7.65 < 0.1);
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

    DO_TEST(test_init_rgb_from_hsv)

     puts("init_rgb_from_hsv_test PASS");
}
