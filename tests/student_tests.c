#include "forces.h"
#include "scene.h"
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "test_util.h"
#include "color.h"
#include "body.h"

list_t *make_shape()
{
    list_t *shape = list_init(4, free);
    vector_t *v = malloc(sizeof(*v));
    *v = (vector_t){-1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, -1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){+1, +1};
    list_add(shape, v);
    v = malloc(sizeof(*v));
    *v = (vector_t){-1, +1};
    list_add(shape, v);
    return shape;
}

/*
 * In a hypothetical sqare, place two objects on opposite corners.
 * Add gravity between the objects, ensure movement is taking place
 */
void test_gravity()
{
    const double M = 10;
    const double DT = 1e-6;
    // Create very high gravity so the objects pass each other
    const double G = 27182.81828459045235;
    const int STEPS = 1000000;

    const double side = 5;

    body_t *topright = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_centroid(topright, (vector_t){side, side});
    body_t *botleft = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_centroid(botleft, (vector_t){-side, -side});

    scene_t *scene = scene_init();
    scene_add_body(scene, topright);
    scene_add_body(scene, botleft);

    create_newtonian_gravity(scene, G, topright, botleft);

    double last_dist_mag = 0;
    double curr_dist_mag;
    for (int i = 0; i < STEPS; i++)
    {
        vector_t ptop = body_get_centroid(topright);
        vector_t pbot = body_get_centroid(botleft);
        vector_t dist_between = vec_subtract(ptop, pbot);

        // Ensure there is some movement
        curr_dist_mag = vec_magnitude(dist_between);
        assert(curr_dist_mag - last_dist_mag != 0);
        if (i == 1)
        {
            // No matter the parameters, the distance should shrink after first step
            assert(curr_dist_mag < last_dist_mag);
        }
        last_dist_mag = curr_dist_mag;

        // Ensure they're still along the 45 degree horizontal
        vector_t expected_dist_direction = (vector_t){.x = 1, .y = 1};
        assert(vec_cross(dist_between, expected_dist_direction) == 0);

        // Ensure the positions add to zero (same mass)
        vector_t center_mass = vec_add(ptop, pbot);
        assert(vec_isclose(center_mass, VEC_ZERO));

        scene_tick(scene, DT);
    }
    scene_free(scene);
}

void test_spring() {
    const double M = 10;
    const double DT = 1e-6;

    // Creates a very high spring constant
    const double G = 27182.81828459045235;
    const int STEPS = 1000000;

    const double side = 5;

    body_t *topright = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_centroid(topright, (vector_t){side, side});
    body_t *botleft = body_init(make_shape(), M, (rgb_color_t){0, 0, 0});
    body_set_centroid(botleft, (vector_t){-side, -side});

    scene_t *scene = scene_init();
    scene_add_body(scene, topright);
    scene_add_body(scene, botleft);

    create_spring(scene, G, topright, botleft);

    double last_dist_mag = 0;
    double curr_dist_mag;
    for (int i = 0; i < STEPS; i++)
    {
        vector_t ptop = body_get_centroid(topright);
        vector_t pbot = body_get_centroid(botleft);
        vector_t dist_between = vec_subtract(ptop, pbot);

        // Ensure there is some movement
        curr_dist_mag = vec_magnitude(dist_between);
        assert(curr_dist_mag - last_dist_mag != 0);
        if (i == 1)
        {
            // No matter the parameters, the distance should shrink after first step
            assert(curr_dist_mag < last_dist_mag);
        }
        last_dist_mag = curr_dist_mag;

        // Ensure they're still along the 45 degree horizontal
        vector_t expected_dist_direction = (vector_t){.x = 1, .y = 1};
        assert(vec_cross(dist_between, expected_dist_direction) == 0);

        // Ensure the positions add to zero (same mass)
        vector_t center_mass = vec_add(ptop, pbot);
        assert(vec_isclose(center_mass, VEC_ZERO));

        scene_tick(scene, DT);
    }
    scene_free(scene);
}

void test_drag() {
    const double MASS_ONE = 10.0;
    const double MASS_TWO = 4.0;
    const double GAMMA = 0.5;
    const double DT = 1e-6;
    const int STEPS = 1000000;

    const vector_t VELOCITY_ONE = {.x = 10.0, .y = 0.0};
    const vector_t VELOCITY_TWO = {.x = 20.0, .y = 0.0};

    body_t *body1 = body_init(make_shape(), MASS_ONE, (rgb_color_t){0, 0, 0});
    body_t *body2 = body_init(make_shape(), MASS_TWO, (rgb_color_t){0, 0, 0});

    body_set_velocity(body1, VELOCITY_ONE);
    body_set_velocity(body2, VELOCITY_TWO);
    
    scene_t *scene = scene_init();
    scene_add_body(scene, body1);
    scene_add_body(scene, body2);

    create_drag(scene, GAMMA, body1);
    create_drag(scene, GAMMA, body2);

    for (int i=0; i<STEPS; i++) {
        vector_t velocity = body_get_velocity(body1);
        vector_t drag = vec_multiply(-1 * GAMMA, velocity);
        vector_t accel1 = vec_multiply(1.0 / body_get_mass(body1), drag);
        vector_t dv1 = vec_multiply(DT, accel1);

        vector_t velocity_2 = body_get_velocity(body2);
        vector_t drag_2 = vec_multiply(-1 * GAMMA, velocity_2);
        vector_t accel2 = vec_multiply(1.0 / body_get_mass(body2), drag_2);
        vector_t dv2 = vec_multiply(DT, accel2);

        vector_t vi1 = body_get_velocity(body1);
        vector_t vf1 = vec_add(vi1, dv1);

        vector_t vi2 = body_get_velocity(body2);
        vector_t vf2 = vec_add(vi2, dv2);

        body_set_velocity(body1, vf1);
        body_set_velocity(body2, vf2);

        assert(vec_isclose(body_get_velocity(body1), vf1));
        assert(vec_isclose(body_get_velocity(body2), vf2));
        
        scene_tick(scene, DT);

    }
    scene_free(scene);
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

    DO_TEST(test_gravity);
    DO_TEST(test_spring);
    DO_TEST(test_drag);

    puts("student_tests PASS");
}
