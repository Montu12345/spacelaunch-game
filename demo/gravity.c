#include "pacman.h"
#include "scene.h"
#include "sprite.h"
#include "sdl_wrapper.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/** @file gravity.c
 *  @brief Function implementations for a bounce animation
 *
 *  This contains the function implementations for a star
 *  bouncing animation.
 *
 *  @author Alexis Wang, Ian Fowler, Ezra Johnson, Shevali Kadakia
 *  @bug No known bugs.
 */

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;

const int STAR_NUM_POINTS = 5;
const double STAR_MIN_LENGTH = 10.0;
const double STAR_MAX_LENGTH = 20.0;
const double STAR_MASS = 120;
const double COEFFICIENT_OF_RESTITUTION = 0.8;
const double STAR_ADD_PERIOD = 1.0;

const rgb_color_t GROUND_COLOR = {.r = 0.2, .g = 0.2, .b = 0.2};

const double INITIAL_VELOCITY_X = 600;
const double INITIAL_VELOCITY_Y = 0;
const double ANGULAR_VELOCITY = 0.003;

const double INITIAL_POSITION_X = -10;
const double INITIAL_POSITION_Y = SCREEN_SIZE_Y;

const vector_t ACCELERATION = {.x = 0, .y = -5000};

const double GROUND_HEIGHT = 50;
const double SCENE_RIGHT_BOUDNARY = ((double)SCREEN_SIZE_X);

const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

scene_t *scene;

void add_star(size_t num_points)
{
    list_t *star_shape = sprite_make_star(num_points, STAR_MIN_LENGTH, STAR_MAX_LENGTH);
    rgb_color_t color = rgb_init_random_bright();
    vector_t initial_position = {.x = INITIAL_POSITION_X, .y = INITIAL_POSITION_Y};
    vector_t initial_velocity = {.x = INITIAL_VELOCITY_X, .y = INITIAL_VELOCITY_Y};
    body_t *star = body_init(star_shape, STAR_MASS, color);
    body_set_centroid(star, initial_position);
    body_set_velocity(star, initial_velocity);
    body_set_angular_velocity(star, ANGULAR_VELOCITY);
    body_set_movable(star, true);
    scene_add_body(scene, star);
}

void add_ground()
{
    list_t *ground_shape = sprite_make_rect(0, SCREEN_SIZE_X, 0, GROUND_HEIGHT);
    vector_t initial_position = {.x = SCREEN_SIZE_X / 2.0, .y = GROUND_HEIGHT / 2.0};
    body_t *ground = body_init(ground_shape, 0, GROUND_COLOR);
    body_set_centroid(ground, initial_position);
    body_set_movable(ground, false);
    scene_add_body(scene, ground);
}

bool remove_body_if_escaped(body_t *body, size_t idx)
{
    if (body_get_centroid(body).x > SCENE_RIGHT_BOUDNARY)
    {
        scene_remove_body(scene, idx);
        return true;
    }
    return false;
}

void accelerate_body(body_t *body, double dt)
{
    vector_t velocity = body_get_velocity(body);
    vector_t dv = vec_multiply(dt, ACCELERATION);
    vector_t vf = vec_add(velocity, dv);
    body_set_velocity(body, vf);
}

bool body_touches_ground(body_t *body)
{
    double cutoff_height = GROUND_HEIGHT + STAR_MAX_LENGTH;
    return body_get_centroid(body).y <= cutoff_height;
}

void bounce_body(body_t *body)
{
    if (!body_touches_ground(body))
    {
        return;
    }
    vector_t vi = body_get_velocity(body);
    if (vi.y >= 0)
    {
        return;
    }
    vector_t reflected = (vector_t){.x = vi.x, .y = -vi.y};
    vector_t vf = vec_multiply(COEFFICIENT_OF_RESTITUTION, reflected);
    body_set_velocity(body, vf);
}

void update_bodies(double dt)
{
    body_t *body;
    for (size_t idx = 0; idx < scene_bodies(scene); idx++)
    {
        body = scene_get_body(scene, idx);
        if (body_is_movable(body))
        {
            accelerate_body(body, dt);
            bounce_body(body);
        }
    }
    for (size_t idx = 0; idx < scene_bodies(scene); idx++)
    {
        if (remove_body_if_escaped(body, idx))
        {
            idx--;
        }
    }
}

int main(int argc, char *argv[])
{
    scene = scene_init();

    size_t num_points = 2;

    add_ground();
    add_star(num_points);

    sdl_init(min, max);
    double dt;
    double time_until_add = STAR_ADD_PERIOD;
    while (!sdl_is_done())
    {
        dt = time_since_last_tick();
        time_until_add -= dt;
        if (time_until_add <= 0)
        {
            num_points++;
            add_star(num_points);
            time_until_add = STAR_ADD_PERIOD;
        }
        update_bodies(dt);
        scene_tick(scene, dt);
        sdl_clear();
        sdl_render_scene(scene);
        sdl_show();
    }
    scene_free(scene);
}
