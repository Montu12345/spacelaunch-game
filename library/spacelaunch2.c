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

void make_dot_2(scene_t *scene, body_t *pacman)
{
    list_t *circle = sprite_make_circle(CIRCLE_PRECISION_2);
    rgb_color_t color;
    enum body_type_t *obstacle_type;
    if (rand() % 2 == 0){
        color = BAD_DOT_COLOR_2;
        obstacle_type = body_type_init_2(good_obstacle_t);
    }
    else{
        color = GOOD_DOT_COLOR_2;
        obstacle_type = body_type_init_2(bad_obstacle_t);
    }
    body_t *dot = body_init_with_info(circle, DOT_MASS_2, color, obstacle_type, free);
    vector_t position = {.x = (rand() % SCREEN_SIZE_X_2), .y = rand() % SCREEN_SIZE_Y_2};
    body_set_centroid(dot, position);
    body_set_movable(dot, false);
    // body_set_camera_mode(dot, SCENE);
    // create_gravity_rocket_obstacles(scene, pacman, dot);
    create_collision_rocket_obstacles_2(scene, pacman, dot);
    scene_add_body(scene, dot);
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

void make_background_color_2(scene_t *scene){
    list_t *background_list = sprite_make_rect(0, 
                                               MAX_OBSTACLES_SCREEN_SIZE_X_2, 
                                               0, 
                                               MAX_OBSTACLES_SCREEN_SIZE_Y_2);
    body_t *background = body_init_with_info(background_list, 
                                             INFINITY, 
                                             BACKGROUND_COLOR_2, 
                                             body_type_init_2(background_t), 
                                             free);
    scene_add_body(scene, background);
}

void make_background_stars_2(scene_t *scene){
    
    for (int i = 0; i < MAX_OBSTACLES_SCREEN_SIZE_Y_2 / FREQUNCY_FOR_STARS_2; i++){
        for (int j = 0; j < MAX_OBSTACLES_SCREEN_SIZE_X_2 / FREQUNCY_FOR_STARS_2; j++){
            list_t *star_list = sprite_make_star(STAR_NUM_OF_POINTS_2, 
                                                 STAR_MIN_LENGTH_2, 
                                                 STAR_MAX_LENGTH_2);
            body_t *star = body_init_with_info(star_list, 
                                               INFINITY, 
                                               STAR_COLOR_2, 
                                               body_type_init_2(star_t), 
                                               free);
            vector_t pos = {.x = i * DISTANCE_BETWEEN_STARS_2, 
                            .y = j * DISTANCE_BETWEEN_STARS_2 + (i % 2) * DISTANCE_BETWEEN_STARS_2/2.0};
            body_set_centroid(star, pos);
            scene_add_body(scene, star);
        }
    }
}

body_t *make_pacman_2(scene_t *scene){
    list_t *pacman_shape = sprite_make_pacman(PACMAN_PRECISION_2);
    body_t *pacman = body_init_with_info(pacman_shape, 
                                         PACMAN_MASS_2, 
                                         PACMAN_COLOR_2, 
                                         body_type_init_2(rocket_t), 
                                         free);
    body_set_centroid(pacman, INITIAL_POS_2);
    body_set_movable(pacman, true);
    // body_set_camera_mode(pacman, FOLLOW);
    scene_add_body(scene, pacman);
    return pacman;
}

void make_shooting_star_2(scene_t *scene){
    list_t *shooting_star_list = sprite_make_circle(3);
    body_t *shooting_star = body_init_with_info(shooting_star_list, 
                                                INFINITY, 
                                                SHOOTING_STAR_COLOR_2, 
                                                body_type_init_2(star_t), 
                                                free);
    vector_t pos = {.x = 0, .y = rand() % MAX_OBSTACLES_SCREEN_SIZE_X_2};
    vector_t velocity = SHOOTING_STAR_VELCOITY_2;
    body_set_velocity(shooting_star, velocity);
    body_set_centroid(shooting_star, pos);
    scene_add_body(scene, shooting_star);
}

void make_moons_2(scene_t *scene, body_t *pacman){
    for (int i = 0; i < INITIAL_DOTS_2; i++){
        make_dot_2(scene, pacman);
    }
}

void make_background_2(scene_t *scene){
    make_background_color_2(scene);
    make_background_stars_2(scene);
}


space_aux_t *game_restart_aux_2(space_aux_t *aux, body_t *pacman){
    aux->game_state = STARTING_KEY_VALUE_2;
    aux->pacman = pacman;
    return aux;
}