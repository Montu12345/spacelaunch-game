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
#include "spacelaunch2.h"

const vector_t ACCELERATION_2 = {.x = 0, .y = -5000};

const int SCREEN_SIZE_X_2 = 1000;
const int SCREEN_SIZE_Y_2 = 500;

const int MAX_OBSTACLES_SCREEN_SIZE_X_2 = 2000;
const int MAX_OBSTACLES_SCREEN_SIZE_Y_2 = 1000;
const int MIN_OBSTACLES_SCREEN_SIZE_Y_2 = -50;

const vector_t min_2 = {.x = 0, .y = 0};
const vector_t max_2 = {.x = SCREEN_SIZE_X_2, .y = SCREEN_SIZE_Y_2};

const int PACMAN_STEP_2 = 10;
const int PACMAN_PRECISION_2 = 30;
const double PACMAN_VELOCITY_SCALE_2 = 30;
const double ROCKET_ELASTICITY_2 = 0.5;
const double VELOCITY_BOOST_2 = 1.0;
const double PACMAN_MASS_2 = 100;

const rgb_color_t PACMAN_COLOR_2 = {.r = 0, .g = 0, .b = 0};
const vector_t INITIAL_POS_2 = {.x = SCREEN_SIZE_X_2 / 10, .y = SCREEN_SIZE_Y_2 / 10};

const double DOT_ADD_PERIOD_2 = 1.0;
const int INITIAL_DOTS_2 = 5;
const int CIRCLE_PRECISION_2 = 50;
const double DOT_MASS_2 = INFINITY;

const rgb_color_t BAD_DOT_COLOR_2 = {.r = 0, .g = 0, .b = 0};
const rgb_color_t GOOD_DOT_COLOR_2 = {.r = 0, .g = 1, .b = 0};

const rgb_color_t BACKGROUND_COLOR_2 = {.r = 0, .g = 0, .b = .55};

const rgb_color_t SHOOTING_STAR_COLOR_2 = {.r = 1, .g = 1, .b = 1};
const vector_t SHOOTING_STAR_VELCOITY_2 = {.x = 700, .y = 0};
const int SHOOTING_STAR_TIME_2 = 170;
// .r = 0.89, .g = 0.35, .b = 0.13

const int STARTING_KEY_VALUE_2 = 0;
const int A_KEY_VALUE_2 = 1; 
const int Q_KEY_VALUE_2 = 2;

const rgb_color_t RED_2 = {.r = 1, .g = 0, .b = 0};
const rgb_color_t YELLOW_2 = {.r = 1, .g = 1, .b = 0};
const rgb_color_t WAIT_BACKGROUND_COLOR_2 = {.r = 0, .g = 0, .b = 0};
const int WAIT_BACKGROUND_TIME_2 = 200;

const rgb_color_t STAR_COLOR_2 = {.r = 1, .g = 1, .b = 1};
const int DISTANCE_BETWEEN_STARS_2 = 125;
const double FREQUNCY_FOR_STARS_2 = 50.0;
const int STAR_NUM_OF_POINTS_2 = 4;
const int STAR_MIN_LENGTH_2 = 2;
const int STAR_MAX_LENGTH_2 = 4;

enum body_type_t
{
    good_obstacle_t,
    bad_obstacle_t,
    rocket_t,
    background_t,
    star_t,
    shooting_star_t,
};

enum body_type_t *body_type_init_2(enum body_type_t b)
{
    enum body_type_t *body_type = malloc(sizeof(enum body_type_t));
    *body_type = b;
    return body_type;
}

typedef struct space_aux
{
    body_t *pacman;
    int game_state;
} space_aux_t;

// game_state
space_aux_t *space_aux_init_2(body_t *pacman, bool game_state)
{
    space_aux_t *aux = malloc(sizeof(space_aux_t));
    *aux = (space_aux_t){
        .pacman = pacman,
        .game_state = game_state,
    };
    return aux;
}

vector_t camera_offset_func_2(body_t *focal_body, void *aux)
{
    vector_t center = vec_multiply(0.5, max_2);
    return vec_subtract(center, body_get_centroid(focal_body));
}

vector_t camera_mover_func_2(vector_t offset, body_t *body)
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

void move_rocket_2(double angle, double scale, body_t *pacman)
{
    vector_t i = (vector_t){PACMAN_STEP_2 * scale, 0};
    vector_t move_vector = vec_rotate(i, angle);
    body_set_rotation(pacman, angle);
    body_add_impulse(pacman, vec_multiply(PACMAN_VELOCITY_SCALE_2, move_vector));
}

void physics_collision_forcer_2(body_t *pacman, body_t *dot, vector_t axis)
{
    vector_t j1 = impulse_to_body_1(pacman, dot, axis, ROCKET_ELASTICITY_2);
    vector_t j2 = vec_negate(j1);
    body_add_impulse(pacman, j1);
    body_add_impulse(dot, j2);
    if (*(enum body_type_t *)body_get_info(dot) == good_obstacle_t){
        body_set_color(pacman, YELLOW_2);
    }
    else if (*(enum body_type_t *)body_get_info(dot) == bad_obstacle_t){
        body_set_color(pacman, RED_2);
    }
}

void create_collision_rocket_obstacles_2(scene_t *scene, body_t *pacman, body_t *dot)
{
    create_collision(scene, 
                     pacman, 
                     dot, 
                     (collision_handler_t)physics_collision_forcer_2, 
                     NULL, 
                     NULL);
}

bool restart_game_3(body_t *ball)
{
    vector_t pos = body_get_centroid(ball);
    if (pos.y - PACMAN_PRECISION_2 > MAX_OBSTACLES_SCREEN_SIZE_Y_2 || 
        pos.y - PACMAN_PRECISION_2 < MIN_OBSTACLES_SCREEN_SIZE_Y_2)
    {
        return true;
    }
    return false;
}

void clear_scene_2(scene_t *scene)
{
    size_t number_of_boxes = scene_bodies(scene);
    for (int i = 0; i < number_of_boxes; i++)
    {
        body_t *current_body = scene_get_body(scene, i);
        body_remove(current_body);
    }
}




space_aux_t *game_restart_aux_2(space_aux_t *aux, body_t *pacman){
    aux->game_state = STARTING_KEY_VALUE_2;
    aux->pacman = pacman;
    return aux;
}