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
const rgb_color_t STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const rgb_color_t SHOOTING_STAR_COLOR = {.r = 0.89, .g = 0.35, .b = 0.13};

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
    bool continue_game;
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

bool should_restart(bool yes_or_no){
    return yes_or_no;
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
        aux->continue_game = true;
        printf("works \n");
        }
        else if (key == Q_KEY)
        {
        aux->continue_game = false;
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
        const rgb_color_t color = {.r = 1, .g = 1, .b = 0};
        body_set_color(pacman, color);
    }
    else if (*(enum body_type_t *)body_get_info(dot) == bad_obstacle_t){
        const rgb_color_t color = {.r = 1, .g = 0, .b = 0};
        body_set_color(pacman, color);
    }
}

void create_collision_rocket_obstacles(scene_t *scene, body_t *pacman, body_t *dot)
{
    create_collision(scene, pacman, dot, (collision_handler_t)physics_collision_forcer_2, NULL, NULL);
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
    for (int i = 0; i < scene_bodies(scene); i++){
        body_t *current_body = scene_get_body(scene, i);
        if (*(enum body_type_t *)body_get_info(current_body) == bad_obstacle_t || *(enum body_type_t *)body_get_info(current_body) == good_obstacle_t){
            vector_t cur_body_pos = body_get_centroid(current_body);
            while(fabs(position.x - cur_body_pos.x) < CIRCLE_PRECISION * 2 || fabs(position.y - cur_body_pos.y) < CIRCLE_PRECISION * 2){
                position = (vector_t){.x = (rand() % SCREEN_SIZE_X), .y = rand() % SCREEN_SIZE_Y};
                // i = -1;
            }
        }
    }
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
    if (pos.y - PACMAN_PRECISION > MAX_OBSTACLES_SCREEN_SIZE_Y || pos.y - PACMAN_PRECISION < -50)
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
    list_t *background_list = sprite_make_rect(0, MAX_OBSTACLES_SCREEN_SIZE_X, 0, MAX_OBSTACLES_SCREEN_SIZE_Y);
    body_t *background = body_init_with_info(background_list, INFINITY, BACKGROUND_COLOR, body_type_init(background_t), free);
    scene_add_body(scene, background);
}

void make_background_stars(scene_t *scene){
    
    for (int i = 0; i < MAX_OBSTACLES_SCREEN_SIZE_Y / 50.0; i++){
        for (int j = 0; j < MAX_OBSTACLES_SCREEN_SIZE_X / 50.0; j++){
            list_t *star_list = sprite_make_star(4, 2, 4);
            body_t *star = body_init_with_info(star_list, INFINITY, STAR_COLOR, body_type_init(star_t), free);
            vector_t pos = {.x = i * 125, .y = j * 125  + (i % 2) * 62.5};
            body_set_centroid(star, pos);
            scene_add_body(scene, star);
        }
    }
}

body_t *make_pacman(scene_t *scene){
    list_t *pacman_shape = sprite_make_pacman(PACMAN_PRECISION);
    body_t *pacman = body_init_with_info(pacman_shape, PACMAN_MASS, PACMAN_COLOR, body_type_init(rocket_t), free);
    body_set_centroid(pacman, INITIAL_POS);
    body_set_movable(pacman, true);
    // body_set_camera_mode(pacman, FOLLOW);
    scene_add_body(scene, pacman);
    return pacman;
}

void *make_shooting_star(scene_t *scene){
    list_t *shooting_star_list = sprite_make_circle(3);
    body_t *shooting_star = body_init_with_info(shooting_star_list, INFINITY, SHOOTING_STAR_COLOR, body_type_init(star_t), free);
    vector_t pos = {.x = 0, .y = rand() % MAX_OBSTACLES_SCREEN_SIZE_X};
    vector_t velocity = (vector_t){.x = 700, .y = 0};
    body_set_velocity(shooting_star, velocity);
    body_set_centroid(shooting_star, pos);
    scene_add_body(scene, shooting_star);
}

int main(int argc, char *argv[])
{
    scene_t *scene = scene_init();
    make_background_color(scene);
    make_background_stars(scene);
    body_t *pacman = make_pacman(scene);
    for (int i = 0; i < INITIAL_DOTS; i++)
    {
        make_dot(scene, pacman);
    }
    space_aux_t *aux = space_aux_init(pacman, true);
    sdl_init(min, max);
    // scene_add_camera_management(scene,
    //                            (camera_offset_func_t)camera_offset_func,
    //                            (camera_mover_func_t)camera_mover_func,
    //                            NULL);
    double dt;
    double time_until_add = DOT_ADD_PERIOD;
    int t = 0;
    while (!sdl_is_done()){
        t += 1;
        sdl_event_args(aux);
        dt = time_since_last_tick();
        time_until_add -= dt;
        int timer = 0;
        if(t % 170 == 0){
            make_shooting_star(scene);
            t = 0;
        }
        if (restart_game(pacman))
        {
            // list_t *background_list = sprite_make_rect(0, SCREEN_SIZE_X, 0, SCREEN_SIZE_Y);
            // body_t *background = body_init(background_list, 0, (rgb_color_t){.r = 0, .g = 0, .b = 0});
            // clear_scene(scene);
            // scene_add_body(scene, background);
            // while (timer < 200){
            //     sdl_on_key((key_handler_t)handle);
            //     if (aux->continue_game){
            //         timer = 200;
            //     }
            //     sdl_render_scene(scene);
            //     // printf("asdf %d \n", timer);
            //     timer += 1;
            //     sdl_show();
            // }
            // scene_tick(scene, dt);
            clear_scene(scene);
            sdl_clear();
            sdl_render_scene(scene);
            sdl_show();
            
            if (!aux->continue_game){
                break;
            }
            make_background_color(scene);
            make_background_stars(scene);
            pacman = make_pacman(scene);
            for (int i = 0; i < INITIAL_DOTS; i++){
                make_dot(scene, pacman);
            }
            aux->continue_game = true;
            aux->pacman = pacman;
            sdl_event_args(aux);
            // create_new_game(scene, scene, paddle);
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
