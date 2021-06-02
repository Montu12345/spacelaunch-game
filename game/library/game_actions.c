#include "game_actions.h"

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
const double KEY_PRESS_VELOCITY_SCALE = 1.0;

const int SCORE_DISPLAY_HEIGHT = 20;
const int SCORE_DISPLAY_WIDTH = 100;

const vector_t SCORE_DISPLAY_OFFSET = {.x = 90, .y = 25};

const int MAX_THRUST_TICKS = 20;

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
    vector_t screen_max = *(vector_t *)aux;
    vector_t center = vec_multiply(0.5, screen_max);
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

void game_setup(game_state_t *state, vector_t screen_min, vector_t screen_max)
{
    scene_t *scene = scene_init();
    game_build_draw_stary_night(scene);

    game_texts_t *texts = malloc(sizeof(game_texts_t));
    state->texts = texts;

    body_t *score_display = game_build_score_keeper(scene, SCORE_DISPLAY_WIDTH, SCORE_DISPLAY_HEIGHT);
    state->scene = scene;
    state->needs_restart = false;
    state->score_display = score_display;
    state->score = 0;
    state->health = 100;
    state->timer = 0;
    state->level = 1;
    body_t *rocket = game_build_rocket(scene, state);
    state->rocket = rocket;
    game_build_draw_asteroids(state, rocket, screen_min, screen_max);
    vector_t *screen_max_aux = vec_malloc(screen_max.x, screen_max.y);
    scene_add_camera_management(state->scene,
                                (camera_offset_func_t)game_actions_camera_offset_func,
                                (camera_mover_func_t)game_actions_camera_mover_func,
                                screen_max_aux,
                                free);
    scene_set_focal_body(scene, state->rocket);
    game_build_display_text(state);
}


void game_actions_thrust_rocket(double angle, double scale, game_state_t *state)
{
    vector_t i = (vector_t){GA_ROCKET_STEP * scale, 0};
    vector_t move_vector = vec_rotate(i, angle);
    body_set_rotation(state->rocket, angle);
    body_add_impulse(state->rocket, vec_multiply(GA_ROCKET_VELOCITY_SCALE, move_vector));
    state->thrust_ticks_remaining = MAX_THRUST_TICKS;
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
            game_actions_thrust_rocket(M_PI * 1.0 / 4, 1, state);
            break;
        case LEFT_ARROW:
            game_actions_thrust_rocket(M_PI, boost, state);
            break;
        case RIGHT_ARROW:
            game_actions_thrust_rocket(0, boost, state);
            break;
        case DOWN_ARROW:
            game_actions_thrust_rocket(M_PI * 3.0 / 2, boost, state);
            break;
        case UP_ARROW:
            game_actions_thrust_rocket(M_PI * 1.0 / 2, boost, state);
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

void game_actions_new_health(game_state_t *state, int scale)
{
    // body_t *curr_display = state->score_display;
    // vector_t position = body_get_centroid(curr_display);
    for (int i = 0; i < scene_bodies(state->scene); i++)
    {
        body_t *display = scene_get_body(state->scene, i);
        if (*(enum space_body_type_t *)body_get_info(display) == SCORE_DISPLAY)
        {
            scene_remove_body(state->scene, i);
        }
    }
    body_t *score_display;
    state->health += scale;
    score_display = game_build_score_keeper(state->scene, state->health, SCORE_DISPLAY_HEIGHT);
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
        game_actions_new_health(state, 10);
    }
    else if (*(enum space_body_type_t *)body_get_info(asteroid) == BAD_OBSTACLE)
    {
        game_actions_new_health(state, -10);
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

// TODO
void game_actions_check_for_win(game_state_t *state)
{
    // vector_t pos = body_get_centroid(state->rocket);
    // if position is at endzone
    // increase level
    // restart game
    state->current_screen = SCREEN_GAME_OVER;
    state->needs_restart = true;
}