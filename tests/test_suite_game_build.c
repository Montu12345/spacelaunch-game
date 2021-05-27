#include "scene.h"
#include "test_util.h"
#include <assert.h>
#include <math.h>
#include <stdlib.h>

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

    // DO_TEST(test_empty_scene)
    // DO_TEST(test_scene)
    // DO_TEST(test_force_creator)
    // DO_TEST(test_force_creator_aux)
    // DO_TEST(test_reaping)

    puts("scene_test PASS");
}