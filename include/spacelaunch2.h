#ifndef __SPACELAUNCH2_H__
#define __SPACELAUNCH2_H__

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

// const vector_t ACCELERATION;

// const int SCREEN_SIZE_X;
// const int SCREEN_SIZE_Y;

// const int MAX_OBSTACLES_SCREEN_SIZE_X;
// const int MAX_OBSTACLES_SCREEN_SIZE_Y;
// const int MIN_OBSTACLES_SCREEN_SIZE_Y;

// const vector_t min;
// const vector_t max;

// const int PACMAN_STEP;
// const int PACMAN_PRECISION;
// const double PACMAN_VELOCITY_SCALE;
// const double ROCKET_ELASTICITY;
// const double VELOCITY_BOOST = 1.0;
// const double PACMAN_MASS = 100;

// const rgb_color_t PACMAN_COLOR = {.r = 0, .g = 0, .b = 0};
// const vector_t INITIAL_POS = {.x = SCREEN_SIZE_X / 10, .y = SCREEN_SIZE_Y / 10};

// const double DOT_ADD_PERIOD = 1.0;
// const int INITIAL_DOTS = 5;
// const int CIRCLE_PRECISION = 50;
// const double DOT_MASS = INFINITY;

// const rgb_color_t BAD_DOT_COLOR = {.r = 0, .g = 0, .b = 0};
// const rgb_color_t GOOD_DOT_COLOR = {.r = 0, .g = 1, .b = 0};

// const rgb_color_t BACKGROUND_COLOR = {.r = 0, .g = 0, .b = .55};

// const rgb_color_t SHOOTING_STAR_COLOR = {.r = 1, .g = 1, .b = 1};
// const vector_t SHOOTING_STAR_VELCOITY = {.x = 700, .y = 0};
// const int SHOOTING_STAR_TIME = 170;
// // .r = 0.89, .g = 0.35, .b = 0.13

// const int STARTING_KEY_VALUE = 0;
// const int A_KEY_VALUE = 1; 
// const int Q_KEY_VALUE = 2;

// const rgb_color_t RED = {.r = 1, .g = 0, .b = 0};
// const rgb_color_t YELLOW = {.r = 1, .g = 1, .b = 0};
// const rgb_color_t WAIT_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};
// const int WAIT_BACKGROUND_TIME = 200;

// const rgb_color_t STAR_COLOR = {.r = 1, .g = 1, .b = 1};
// const int DISTANCE_BETWEEN_STARS = 125;
// const double FREQUNCY_FOR_STARS = 50.0;
// const int STAR_NUM_OF_POINTS = 4;
// const int STAR_MIN_LENGTH = 2;
// const int STAR_MAX_LENGTH = 4;

/**
 * A collection of bodies and force creators.
 * The scene automatically resizes to store
 * arbitrarily many bodies and force creators.
 */
typedef struct space_aux space_aux_t;

// game_state
space_aux_t *space_aux_init_2(
    body_t *pacman, 
    bool game_state);

vector_t camera_offset_func(body_t *focal_body, void *aux);


vector_t camera_mover_func(vector_t offset, body_t *body);


void move_rocket_2(double angle, double scale, body_t *pacman);


void handle(char key, key_event_type_t type, double held_time, space_aux_t *aux);

void physics_collision_forcer_2(body_t *pacman, body_t *dot, vector_t axis);


void create_collision_rocket_obstacles_2(scene_t *scene, body_t *pacman, body_t *dot);


void make_dot_2(scene_t *scene, body_t *pacman);


bool restart_game_3(body_t *ball);


void clear_scene_2(scene_t *scene);


void make_background_color_2(scene_t *scene);

void make_background_stars_2(scene_t *scene);

body_t *make_pacman_2(scene_t *scene);

void make_shooting_star_2(scene_t *scene);

// game shouldn't quit itself
// dont use the timer to detect when key has been pressed
int continue_game_2(scene_t *scene, space_aux_t *aux);

void make_moons_2(scene_t *scene, body_t *pacman);


void make_background_2(scene_t *scene);

void restart_game_2(double dt, scene_t *scene);

space_aux_t *game_restart_aux_2(space_aux_t *aux, body_t *pacman);

#endif // #ifndef __SPACELAUNCH2_H__