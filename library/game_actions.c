#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "collision.h"

const int GA_SCREEN_SIZE_X = 1000;
const int GA_SCREEN_SIZE_Y = 500;
const vector_t GA_min = {.x = 0, .y = 0};
const vector_t GA_max = {.x = GA_SCREEN_SIZE_X, .y = GA_SCREEN_SIZE_Y};
const int GA_STARTING_KEY_VALUE = 0;
const double GA_ROCKET_ELASTICITY = 0.5;
const int GA_ROCKET_STEP = 10;
const double GA_ROCKET_VELOCITY_SCALE = 30;
const rgb_color_t GA_RED = {.r = 1, .g = 0, .b = 0};
const rgb_color_t GA_YELLOW = {.r = 1, .g = 1, .b = 0};
const int GA_ROCKET_RADIUS = 30;
const int GA_MAX_OBSTACLES_SCREEN_SIZE_X = 2000;
const int GA_MAX_OBSTACLES_SCREEN_SIZE_Y = 1000;
const int GA_MIN_OBSTACLES_SCREEN_SIZE_Y = -50;

enum body_type_t
{
    good_obstacle_t,
    bad_obstacle_t,
    rocket_t,
    background_t,
    star_t,
    shooting_star_t,
};

typedef struct space_aux
{
    body_t *rocket;
    int game_state;
} space_aux_t;

vector_t game_actions_camera_offset_func_2(body_t *focal_body, void *aux)
{
    vector_t center = vec_multiply(0.5, GA_max);
    return vec_subtract(center, body_get_centroid(focal_body));
}

vector_t game_actions_camera_mover_func_2(vector_t offset, body_t *body)
{
    camera_mode_t camera_mode = body_get_camera_mode(body);
    switch (camera_mode)
    {
    case FOLLOW:
        return offset;
        break;
    case SCENE:
        return offset;
        break;
    default:
        return VEC_ZERO;
        break;
    }
}

void game_actions_move_rocket(double angle, double scale, body_t *rocket)
{
    vector_t i = (vector_t){GA_ROCKET_STEP * scale, 0};
    vector_t move_vector = vec_rotate(i, angle);
    body_set_rotation(rocket, angle);
    body_add_impulse(rocket, vec_multiply(GA_ROCKET_VELOCITY_SCALE, move_vector));
}

void game_actions_physics_collision(body_t *rocket, body_t *asteroid, vector_t axis)
{
    vector_t j1 = impulse_to_body_1(rocket, asteroid, axis, GA_ROCKET_ELASTICITY);
    vector_t j2 = vec_negate(j1);
    body_add_impulse(rocket, j1);
    body_add_impulse(asteroid, j2);
    if (*(enum body_type_t *)body_get_info(asteroid) == good_obstacle_t){
        body_set_color(rocket, GA_YELLOW);
    }
    else if (*(enum body_type_t *)body_get_info(asteroid) == bad_obstacle_t){
        body_set_color(rocket, GA_RED);
    }
}

void game_actions_rocket_obstacles_collision(scene_t *scene, body_t *rocket, body_t *asteroid)
{
    create_collision(scene, 
                     rocket, 
                     asteroid, 
                     (collision_handler_t)game_actions_physics_collision, 
                     NULL, 
                     NULL);
}

bool game_actions_restart_game(body_t *rocket)
{
    vector_t pos = body_get_centroid(rocket);
    if (pos.y - GA_ROCKET_RADIUS > GA_MAX_OBSTACLES_SCREEN_SIZE_Y || 
        pos.y - GA_ROCKET_RADIUS < GA_MIN_OBSTACLES_SCREEN_SIZE_Y)
    {
        return true;
    }
    return false;
}

void game_actions_clear_scene(scene_t *scene)
{
    size_t number_of_boxes = scene_bodies(scene);
    for (int i = 0; i < number_of_boxes; i++)
    {
        body_t *current_body = scene_get_body(scene, i);
        body_remove(current_body);
    }
}

space_aux_t *game_actions_game_restart_aux(space_aux_t *aux, body_t *rocket){
    aux->game_state = GA_STARTING_KEY_VALUE;
    aux->rocket = rocket;
    return aux;
}