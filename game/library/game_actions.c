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


enum space_body_type_t
{
    GOOD_OBSTACLE,
    BAD_OBSTACLE,
    ROCKET,
    BACKGROUND_OBJECT,
    STAR,
    SHOOTING_STAR,
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
    game_build_draw_asteroids(scene, rocket);
    void game_actions_display_score(game_state_t *state);
    body_t *score_display = game_build_score_keeper(scene);

    state->rocket = rocket;
    state->scene = scene;
    state->needs_restart = false;
    state->score_display = score_display;
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

void game_actions_physics_collision(body_t *focal_body, body_t *asteroid, vector_t axis)
{
    vector_t j1 = impulse_to_body_1(focal_body, asteroid, axis, GA_ROCKET_ELASTICITY);
    vector_t j2 = vec_negate(j1);
    body_add_impulse(focal_body, j1);
    body_add_impulse(asteroid, j2);
    if (*(enum space_body_type_t *)body_get_info(asteroid) == GOOD_OBSTACLE)
    {
        body_set_color(focal_body, GA_YELLOW);
    }
    else if (*(enum space_body_type_t *)body_get_info(asteroid) == BAD_OBSTACLE)
    {
        body_set_color(focal_body, GA_RED);
    }
}

void game_actions_rocket_obstacles_collision(scene_t *scene, body_t *focal_body, body_t *asteroid)
{
    create_collision(scene,
                     focal_body,
                     asteroid,
                     (collision_handler_t)game_actions_physics_collision,
                     NULL,
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