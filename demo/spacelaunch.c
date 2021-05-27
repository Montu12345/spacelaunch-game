// #include "camerademo.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL_ttf.h>

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "collision.h"
#include "spacelaunch2.h"
#include "spacelaunch.h"
#include "game_build.h"

/** 
 * Modifications on pacman to test out camera functionality
 */
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};
const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;
const rgb_color_t WAIT_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};
const int STARTING_KEY_VALUE = 0;
const int A_KEY_VALUE = 1; 
const int Q_KEY_VALUE = 2;
const double VELOCITY_BOOST = 1.0;
const int SHOOTING_STAR_TIME = 170;

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
    int game_state;
} space_aux_t;

// game_state
space_aux_t *space_aux_init(body_t *pacman, bool game_state)
{
    space_aux_t *aux = malloc(sizeof(space_aux_t));
    *aux = (space_aux_t){
        .pacman = pacman,
        .game_state = game_state,
    };
    return aux;
}

void handle(char key, key_event_type_t type, double held_time, space_aux_t *aux)
{
    double boost = VELOCITY_BOOST + held_time;
    if (type == KEY_PRESSED)
    {
        if (key == SPACEBAR)
        {
            move_rocket_2(M_PI * 1.0 / 4, 1, aux->pacman);
        }
        else if (key == LEFT_ARROW)
        {
            move_rocket_2(M_PI, boost, aux->pacman);
        }
        else if (key == RIGHT_ARROW)
        {
            move_rocket_2(0, boost, aux->pacman);
        }
        else if (key == DOWN_ARROW)
        {
            move_rocket_2(M_PI * 3.0 / 2, boost, aux->pacman);
        }
        else if (key == UP_ARROW)
        {
            move_rocket_2(M_PI * 1.0 / 2, boost, aux->pacman);
        }
        else if (key == A_KEY)
        {
            aux->game_state = A_KEY_VALUE;
        }
        else if (key == Q_KEY)
        {
            aux->game_state = Q_KEY_VALUE;
        }
    }
}

int continue_game(scene_t *scene, space_aux_t *aux){
    sdl_clear();
    sdl_event_args(aux);
    sdl_on_key((key_handler_t)handle);
    if (aux->game_state == A_KEY_VALUE){
        return A_KEY_VALUE;
        printf("here \n");
    }
    if (aux->game_state == Q_KEY_VALUE){
        // timer = WAIT_BACKGROUND_TIME;
        return Q_KEY_VALUE;
    }
    sdl_render_scene(scene);
    sdl_show();
    return 0;
}

void restart_game_2(double dt, scene_t *scene){
    scene_tick(scene, dt);
    clear_scene_2(scene);
    sdl_clear();
    sdl_render_scene(scene);
    sdl_show();
}

space_aux_t *game_restart_aux(space_aux_t *aux, body_t *pacman){
    aux->game_state = STARTING_KEY_VALUE;
    aux->pacman = pacman;
    return aux;
}

int main(int argc, char *argv[])
{
    double dt;
    int t = 0;
    scene_t *scene = scene_init();
    game_build_draw_stary_night(scene);
    body_t *pacman = game_build_rocket(scene);
    game_build_draw_asteroids(scene, pacman);
    space_aux_t *aux = space_aux_init(pacman, STARTING_KEY_VALUE);
    sdl_init(min, max);
    
    while (!sdl_is_done()){
        t += 1;
        sdl_event_args(aux);
        dt = time_since_last_tick();
        if(t % SHOOTING_STAR_TIME == 0){
            game_build_shooting_star(scene);
            t = 0;
        }
        if (restart_game_3(pacman))
        {
            list_t *background_list = sprite_make_rect(0, 
                                                       SCREEN_SIZE_X, 
                                                       0, 
                                                       SCREEN_SIZE_Y);
            body_t *background = body_init(background_list, 
                                           0, 
                                           WAIT_BACKGROUND_COLOR);
            clear_scene_2(scene);
            scene_add_body(scene, background);
            while (aux->game_state == 0 && !sdl_is_done()){
                create_words();
                aux->game_state = continue_game(scene, aux);
            }
            if (aux->game_state == 2){
                break;
            }
            restart_game_2(dt, scene);
            game_build_draw_stary_night(scene);
            pacman = game_build_rocket(scene);
            game_build_draw_asteroids(scene, pacman);
            aux = game_restart_aux_2(aux, pacman);
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