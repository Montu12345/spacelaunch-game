// #include "camerademo.h"
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


/** 
 * Modifications on pacman to test out camera functionality
 */

const vector_t ACCELERATION = {.x = 0, .y = -5000};

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;

const int MAX_OBSTACLES_SCREEN_SIZE_X = 2000;
const int MAX_OBSTACLES_SCREEN_SIZE_Y = 1000;
const int MIN_OBSTACLES_SCREEN_SIZE_Y = -50;

const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

const int PACMAN_STEP = 10;
const int PACMAN_PRECISION = 30;
const double PACMAN_VELOCITY_SCALE = 30;
const double ROCKET_ELASTICITY = 0.5;
const double VELOCITY_BOOST = 1.0;
const double PACMAN_MASS = 100;

const rgb_color_t PACMAN_COLOR = {.r = 0, .g = 0, .b = 0};
const vector_t INITIAL_POS = {.x = SCREEN_SIZE_X / 10, .y = SCREEN_SIZE_Y / 10};

const double DOT_ADD_PERIOD = 1.0;
const int INITIAL_DOTS = 5;
const int CIRCLE_PRECISION = 50;
const double DOT_MASS = INFINITY;

const rgb_color_t BAD_DOT_COLOR = {.r = 0, .g = 0, .b = 0};
const rgb_color_t GOOD_DOT_COLOR = {.r = 0, .g = 1, .b = 0};

const rgb_color_t BACKGROUND_COLOR = {.r = 0, .g = 0, .b = .55};

const rgb_color_t SHOOTING_STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const vector_t SHOOTING_STAR_VELCOITY = {.x = 700, .y = 0};
const int SHOOTING_STAR_TIME = 170;
// .r = 0.89, .g = 0.35, .b = 0.13

const int STARTING_KEY_VALUE = 0;
const int A_KEY_VALUE = 1; 
const int Q_KEY_VALUE = 2;

const rgb_color_t RED = {.r = 1, .g = 0, .b = 0};
const rgb_color_t YELLOW = {.r = 1, .g = 1, .b = 0};
const rgb_color_t WAIT_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};
const int WAIT_BACKGROUND_TIME = 200;

const rgb_color_t STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const int DISTANCE_BETWEEN_STARS = 125;
const double FREQUNCY_FOR_STARS = 50.0;
const int STAR_NUM_OF_POINTS = 4;
const int STAR_MIN_LENGTH = 2;
const int STAR_MAX_LENGTH = 4;


// const double GRAVITY_CONSTANT = 10;

enum body_type_t
{
    good_obstacle_t,
    bad_obstacle_t,
    rocket_t,
    background_t,
    star_t,
    shooting_star_t,
};

enum body_type_t *body_type_init(enum body_type_t b)
{
    enum body_type_t *body_type = malloc(sizeof(enum body_type_t));
    *body_type = b;
    return body_type;
}

typedef struct space_aux
{
    body_t *pacman;
    int continue_game;
} space_aux_t;

space_aux_t *space_aux_init(body_t *pacman, bool continue_game)
{
    space_aux_t *aux = malloc(sizeof(space_aux_t));
    *aux = (space_aux_t){
        .pacman = pacman,
        .continue_game = continue_game,
    };
    return aux;
}

typedef struct game_future
{
    bool truth;
    double timer;
} game_future_t;

game_future_t *game_future_init(bool truth, double timer)
{
    game_future_t *aux = malloc(sizeof(game_future_t));
    *aux = (game_future_t){
        .truth = truth,
        .timer = timer,
    };
    return aux;
}

vector_t camera_offset_func(body_t *focal_body, void *aux)
{
    vector_t center = vec_multiply(0.5, max);
    return vec_subtract(center, body_get_centroid(focal_body));
}

vector_t camera_mover_func(vector_t offset, body_t *body)
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

void move_rocket(double angle, double scale, body_t *pacman)
{
    vector_t i = (vector_t){PACMAN_STEP * scale, 0};
    vector_t move_vector = vec_rotate(i, angle);
    body_set_rotation(pacman, angle);
    body_add_impulse(pacman, vec_multiply(PACMAN_VELOCITY_SCALE, move_vector));
}

void handle(char key, key_event_type_t type, double held_time, space_aux_t *aux)
{
    double boost = VELOCITY_BOOST + held_time;
    if (type == KEY_PRESSED)
    {
        if (key == SPACEBAR)
        {
            move_rocket(M_PI * 1.0 / 4, 1, aux->pacman);
        }
        else if (key == LEFT_ARROW)
        {
            move_rocket(M_PI, boost, aux->pacman);
        }
        else if (key == RIGHT_ARROW)
        {
            move_rocket(0, boost, aux->pacman);
        }
        else if (key == DOWN_ARROW)
        {
            move_rocket(M_PI * 3.0 / 2, boost, aux->pacman);
        }
        else if (key == UP_ARROW)
        {
            move_rocket(M_PI * 1.0 / 2, boost, aux->pacman);
        }
        else if (key == A_KEY)
        {
            aux->continue_game = A_KEY_VALUE;
        }
        else if (key == Q_KEY)
        {
            aux->continue_game = Q_KEY_VALUE;
        }
    }
}

void physics_collision_forcer_2(body_t *pacman, body_t *dot, vector_t axis)
{
    vector_t j1 = impulse_to_body_1(pacman, dot, axis, ROCKET_ELASTICITY);
    vector_t j2 = vec_negate(j1);
    body_add_impulse(pacman, j1);
    body_add_impulse(dot, j2);
    if (*(enum body_type_t *)body_get_info(dot) == good_obstacle_t){
        body_set_color(pacman, YELLOW);
    }
    else if (*(enum body_type_t *)body_get_info(dot) == bad_obstacle_t){
        body_set_color(pacman, RED);
    }
}

void create_collision_rocket_obstacles(scene_t *scene, body_t *pacman, body_t *dot)
{
    create_collision(scene, 
                     pacman, 
                     dot, 
                     (collision_handler_t)physics_collision_forcer_2, 
                     NULL, 
                     NULL);
}

void make_dot(scene_t *scene, body_t *pacman)
{
    list_t *circle = sprite_make_circle(CIRCLE_PRECISION);
    rgb_color_t color;
    enum body_type_t *obstacle_type;
    if (rand() % 2 == 0){
        color = BAD_DOT_COLOR;
        obstacle_type = body_type_init(good_obstacle_t);
    }
    else{
        color = GOOD_DOT_COLOR;
        obstacle_type = body_type_init(bad_obstacle_t);
    }
    body_t *dot = body_init_with_info(circle, DOT_MASS, color, obstacle_type, free);
    vector_t position = {.x = (rand() % SCREEN_SIZE_X), .y = rand() % SCREEN_SIZE_Y};
    body_set_centroid(dot, position);
    body_set_movable(dot, false);
    // body_set_camera_mode(dot, SCENE);
    // create_gravity_rocket_obstacles(scene, pacman, dot);
    create_collision_rocket_obstacles(scene, pacman, dot);
    scene_add_body(scene, dot);
}

bool restart_game(body_t *ball)
{
    vector_t pos = body_get_centroid(ball);
    if (pos.y - PACMAN_PRECISION > MAX_OBSTACLES_SCREEN_SIZE_Y || 
        pos.y - PACMAN_PRECISION < MIN_OBSTACLES_SCREEN_SIZE_Y)
    {
        return true;
    }
    return false;
}

void clear_scene(scene_t *scene)
{
    size_t number_of_boxes = scene_bodies(scene);
    for (int i = 0; i < number_of_boxes; i++)
    {
        body_t *current_body = scene_get_body(scene, i);
        body_remove(current_body);
    }
}

void make_background_color(scene_t *scene){
    list_t *background_list = sprite_make_rect(0, 
                                               MAX_OBSTACLES_SCREEN_SIZE_X, 
                                               0, 
                                               MAX_OBSTACLES_SCREEN_SIZE_Y);
    body_t *background = body_init_with_info(background_list, 
                                             INFINITY, 
                                             BACKGROUND_COLOR, 
                                             body_type_init(background_t), 
                                             free);
    scene_add_body(scene, background);
}

void make_background_stars(scene_t *scene){
    
    for (int i = 0; i < MAX_OBSTACLES_SCREEN_SIZE_Y / FREQUNCY_FOR_STARS; i++){
        for (int j = 0; j < MAX_OBSTACLES_SCREEN_SIZE_X / FREQUNCY_FOR_STARS; j++){
            list_t *star_list = sprite_make_star(STAR_NUM_OF_POINTS, 
                                                 STAR_MIN_LENGTH, 
                                                 STAR_MAX_LENGTH);
            body_t *star = body_init_with_info(star_list, 
                                               INFINITY, 
                                               STAR_COLOR, 
                                               body_type_init(star_t), 
                                               free);
            vector_t pos = {.x = i * DISTANCE_BETWEEN_STARS, 
                            .y = j * DISTANCE_BETWEEN_STARS + (i % 2) * DISTANCE_BETWEEN_STARS/2.0};
            body_set_centroid(star, pos);
            scene_add_body(scene, star);
        }
    }
}

body_t *make_pacman(scene_t *scene){
    list_t *pacman_shape = sprite_make_pacman(PACMAN_PRECISION);
    body_t *pacman = body_init_with_info(pacman_shape, 
                                         PACMAN_MASS, 
                                         PACMAN_COLOR, 
                                         body_type_init(rocket_t), 
                                         free);
    body_set_centroid(pacman, INITIAL_POS);
    body_set_movable(pacman, true);
    // body_set_camera_mode(pacman, FOLLOW);
    scene_add_body(scene, pacman);
    return pacman;
}

void make_shooting_star(scene_t *scene){
    list_t *shooting_star_list = sprite_make_circle(3);
    body_t *shooting_star = body_init_with_info(shooting_star_list, 
                                                INFINITY, 
                                                SHOOTING_STAR_COLOR, 
                                                body_type_init(star_t), 
                                                free);
    vector_t pos = {.x = 0, .y = rand() % MAX_OBSTACLES_SCREEN_SIZE_X};
    vector_t velocity = SHOOTING_STAR_VELCOITY;
    body_set_velocity(shooting_star, velocity);
    body_set_centroid(shooting_star, pos);
    scene_add_body(scene, shooting_star);
}

game_future_t *continue_game(double timer, scene_t *scene, space_aux_t *aux, bool truth){
    timer += time_since_last_tick();
    sdl_clear();
    sdl_event_args(aux);
    sdl_on_key((key_handler_t)handle);
    if (aux->continue_game == A_KEY_VALUE){
        timer = WAIT_BACKGROUND_TIME;
    }
    if (aux->continue_game == Q_KEY_VALUE){
        timer = WAIT_BACKGROUND_TIME;
        truth = true;
    }
    sdl_render_scene(scene);
    sdl_show();
    game_future_t *game_continue_aux = game_future_init(truth, timer);
    return game_continue_aux;
}

void make_moons(scene_t *scene, body_t *pacman){
    for (int i = 0; i < INITIAL_DOTS; i++){
        make_dot(scene, pacman);
    }
}

void make_background(scene_t *scene){
    make_background_color(scene);
    make_background_stars(scene);
}

void restart_game_2(double dt, scene_t *scene){
    scene_tick(scene, dt);
    clear_scene(scene);
    sdl_clear();
    sdl_render_scene(scene);
    sdl_show();
}

space_aux_t *game_restart_aux(space_aux_t *aux, body_t *pacman){
    aux->continue_game = STARTING_KEY_VALUE;
    aux->pacman = pacman;
    return aux;
}

int main(int argc, char *argv[])
{
    double dt;
    int t = 0;
    bool truth = false;
    scene_t *scene = scene_init();
    make_background(scene);
    body_t *pacman = make_pacman(scene);
    make_moons(scene, pacman);
    space_aux_t *aux = space_aux_init(pacman, STARTING_KEY_VALUE);
    sdl_init(min, max);
    while (!sdl_is_done()){
        t += 1;
        sdl_event_args(aux);
        dt = time_since_last_tick();
        double timer = 0;
        if(t % SHOOTING_STAR_TIME == 0){
            make_shooting_star(scene);
            t = 0;
        }
        if (restart_game(pacman))
        {
            list_t *background_list = sprite_make_rect(0, 
                                                       SCREEN_SIZE_X, 
                                                       0, 
                                                       SCREEN_SIZE_Y);
            body_t *background = body_init(background_list, 
                                           0, 
                                           WAIT_BACKGROUND_COLOR);
            clear_scene(scene);
            scene_add_body(scene, background);
            while (timer < WAIT_BACKGROUND_TIME && !sdl_is_done()){
                timer = continue_game(timer, scene, aux, truth)->timer;
                truth = continue_game(timer, scene, aux, truth)->truth;
            }
            if (truth){
                break;
            }
            restart_game_2(dt, scene);
            make_background(scene);
            pacman = make_pacman(scene);
            make_moons(scene, pacman);
            aux = game_restart_aux(aux, pacman);
            sdl_event_args(aux);
            scene_tick(scene, dt);
        }
        scene_tick(scene, dt);
        sdl_clear();
        sdl_on_key((key_handler_t)handle);
        sdl_render_scene(scene);
        sdl_show();
    }
    scene_free(scene);
}
