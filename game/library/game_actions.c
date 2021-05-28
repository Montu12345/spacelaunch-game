#include "game_actions.h"

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
const int A_KEY_VALUE = 1;
const int Q_KEY_VALUE = 2;
const double KEY_PRESS_VELOCITY_SCALE = 1.0;

const int SCORE_DISPLAY_HEIGHT = 20;
const int SCORE_DISPLAY_WIDTH = 150;
const vector_t SCORE_DISPLAY_POSITION = {.x = 90, .y = GA_SCREEN_SIZE_Y - 25};

enum space_body_type_t
{
    GOOD_OBSTACLE,
    BAD_OBSTACLE,
    ROCKET,
    BACKGROUND_OBJECT,
    STAR,
    SHOOTING_STAR,
    SCORE_DISPLAY,
};

vector_t game_actions_camera_offset_func(body_t *focal_body, void *aux)
{
    vector_t center = vec_multiply(0.5, GA_max);
    return vec_subtract(center, body_get_centroid(focal_body));
}

vector_t game_actions_camera_mover_func(vector_t offset, body_t *body)
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

void game_setup(game_state_t *state)
{
    scene_t *scene = scene_init();
    game_build_draw_stary_night(scene);
    body_t *rocket = game_build_rocket(scene);
    body_t *score_display = game_build_score_keeper(scene, SCORE_DISPLAY_WIDTH, SCORE_DISPLAY_HEIGHT, SCORE_DISPLAY_POSITION);
    state->rocket = rocket;
    state->scene = scene;
    state->needs_restart = false;
    state->score_display = score_display;
    state->score = 0;
    state->health = 100;
    state->timer = 0;
    game_build_draw_asteroids(state, rocket);
    scene_add_camera_management(state->scene,
                              (camera_offset_func_t)game_actions_camera_offset_func,
                              (camera_mover_func_t)game_actions_camera_mover_func,
                              NULL);
    scene_set_focal_body(scene, state->rocket);
}


void game_actions_thrust_rocket(double angle, double scale, body_t *rocket)
{
    vector_t i = (vector_t){GA_ROCKET_STEP * scale, 0};
    vector_t move_vector = vec_rotate(i, angle);
    body_set_rotation(rocket, angle);
    body_add_impulse(rocket, vec_multiply(GA_ROCKET_VELOCITY_SCALE, move_vector));
}

void handle_key_press(char key, key_event_type_t type, double held_time, game_state_t *state)
{
    double boost = KEY_PRESS_VELOCITY_SCALE + held_time;
    if (type != KEY_PRESSED)
    {
        return;
    }

    if (state->current_screen == SCREEN_GAME)
    {
        switch (key)
        {
        case SPACEBAR:
            game_actions_thrust_rocket(M_PI * 1.0 / 4, 1, state->rocket);
            break;
        case LEFT_ARROW:
            game_actions_thrust_rocket(M_PI, boost, state->rocket);
            break;
        case RIGHT_ARROW:
            game_actions_thrust_rocket(0, boost, state->rocket);
            break;
        case DOWN_ARROW:
            game_actions_thrust_rocket(M_PI * 3.0 / 2, boost, state->rocket);
            break;
        case UP_ARROW:
            game_actions_thrust_rocket(M_PI * 1.0 / 2, boost, state->rocket);
            break;
        default:
            break;
        }
    }
    else if (state->current_screen == SCREEN_GAME_OVER)
    {
        if (key == A_KEY)
        {
            state->current_screen = SCREEN_GAME;
            state->needs_restart = true;
        }
    }
}

void game_actions_new_health(game_state_t *state, int scale){
    body_t *curr_display = state->score_display;
    list_t *curr_display_list = body_get_shape(curr_display);
    vector_t *curr_length = (vector_t *)list_get(curr_display_list, 3);
    vector_t position = body_get_centroid(curr_display);
    for (int i = 0; i < scene_bodies(state->scene); i++){
        body_t *display = scene_get_body(state->scene, i);
        if (*(enum space_body_type_t *)body_get_info(display) == SCORE_DISPLAY){
            scene_remove_body(state->scene, i);
        }
    }
    vector_t new_position;
    body_t *score_display;
    printf("%f \n", curr_length->x);
    if(curr_length->x + scale <= 0){
        printf("need to end game");
    }
    // if (curr_length->x + scale >= 200.0){
        
    // }
    // else{
    //     new_position = (vector_t){.x = (curr_length->x + scale) / 2.0 + 15, .y = position.y};
    //     score_display = game_build_score_keeper(state->scene, curr_length->x + scale, SCORE_DISPLAY_HEIGHT, new_position);
    // }
    state->health += scale;
    new_position = (vector_t){.x = curr_length->x / 2.0, .y = position.y};
    score_display = game_build_score_keeper(state->scene, curr_length->x, SCORE_DISPLAY_HEIGHT, new_position);
    state->score_display = score_display;
}

void game_actions_physics_collision(body_t *focal_body, body_t *asteroid, vector_t axis, game_state_t *state)
{
    vector_t j1 = impulse_to_body_1(focal_body, asteroid, axis, GA_ROCKET_ELASTICITY);
    vector_t j2 = vec_negate(j1);
    body_add_impulse(focal_body, j1);
    body_add_impulse(asteroid, j2);
    if (*(enum space_body_type_t *)body_get_info(asteroid) == GOOD_OBSTACLE)
    {
        game_actions_new_health(state, 100);
    }
    else if (*(enum space_body_type_t *)body_get_info(asteroid) == BAD_OBSTACLE)
    {
        game_actions_new_health(state, -50);
    }
}

void game_actions_rocket_obstacles_collision(scene_t *scene, body_t *focal_body, body_t *asteroid, game_state_t *state)
{
    create_collision(scene,
                     focal_body,
                     asteroid,
                     (collision_handler_t)game_actions_physics_collision,
                     state,
                     NULL);
}

void game_actions_check_for_game_over(game_state_t *state)
{
    vector_t pos = body_get_centroid(state->rocket);
    if (pos.y - GA_ROCKET_RADIUS > GA_MAX_OBSTACLES_SCREEN_SIZE_Y ||
        pos.y - GA_ROCKET_RADIUS < GA_MIN_OBSTACLES_SCREEN_SIZE_Y)
    {
        state->current_screen = SCREEN_GAME_OVER;
        state->needs_restart = true;
    }
}