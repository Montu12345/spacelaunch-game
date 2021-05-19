#include "scene.h"
#include "body.h"
#include "list.h"
#include "sprite.h"
#include "color.h"
#include "sdl_wrapper.h"
#include <time.h>
#include "forces.h"
#include <stdio.h>

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;

const int STAR_MIN_LENGTH = 10;
const int STAR_MAX_LENGTH = 1;
const double BALL_MASS = 10;
const int NUMBER_OF_STARS = 20;
const int MAX_VELCOITY = 10;
const double GRAVITY_CONSTANT = 4000;

const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

void make_bodies(scene_t *scene)
{
    int negate = -1;
    for (int i = 0; i < NUMBER_OF_STARS; i++)
    {
        int length = rand() % STAR_MIN_LENGTH + 10;
        list_t *star = sprite_make_star(4, length, length + STAR_MAX_LENGTH * length);
        rgb_color_t star_color = rgb_init_random_bright();
        body_t *body = body_init(star, BALL_MASS * length * length, star_color);
        if (i % 2 == 0)
        {
            negate *= -1;
        }
        vector_t position = (vector_t){.x = rand() % SCREEN_SIZE_X, .y = rand() % SCREEN_SIZE_Y};
        vector_t velocity = (vector_t){.x = negate * (rand() % MAX_VELCOITY), .y = negate * (rand() % MAX_VELCOITY)};
        body_set_velocity(body, velocity);
        body_set_centroid(body, position);
        scene_add_body(scene, body);
    }
}

void add_gravity(scene_t *scene)
{
    for (size_t i = 0; i < NUMBER_OF_STARS; i++)
    {
        for (size_t j = 0; j < i; j++)
        {
            body_t *current_body = scene_get_body(scene, i);
            body_t *next_body = scene_get_body(scene, j);
            create_newtonian_gravity(scene, GRAVITY_CONSTANT, current_body, next_body);
        }
    }
}

int main()
{
    scene_t *scene = scene_init();
    make_bodies(scene);
    sdl_init(min, max);
    // srand(time(0));
    add_gravity(scene);

    while (!sdl_is_done())
    {
        double dt = time_since_last_tick();
        scene_tick(scene, dt);
        sdl_clear();
        sdl_render_scene(scene);
        sdl_show();
    }
    scene_free(scene);
}