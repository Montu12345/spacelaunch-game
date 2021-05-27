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


game_state_t *game_state_init(body_t *focal_body, bool game_state)
{
    game_state_t *aux = malloc(sizeof(game_state_t));
    *aux = (game_state_t){
        .focal_body = focal_body,
        .game_state_num = game_state,
    };
    return aux;
}

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

void game_actions_move_rocket(double angle, double scale, body_t *focal_body)
{
    vector_t i = (vector_t){GA_ROCKET_STEP * scale, 0};
    vector_t move_vector = vec_rotate(i, angle);
    body_set_rotation(focal_body, angle);
    body_add_impulse(focal_body, vec_multiply(GA_ROCKET_VELOCITY_SCALE, move_vector));
}

void handle_key_press(char key, key_event_type_t type, double held_time, game_state_t *aux)
{
    double boost = KEY_PRESS_VELOCITY_SCALE + held_time;
    if (type == KEY_PRESSED)
    {
        if (key == SPACEBAR)
        {
            game_actions_move_rocket(M_PI * 1.0 / 4, 1, aux->focal_body);
        }
        else if (key == LEFT_ARROW)
        {
            game_actions_move_rocket(M_PI, boost, aux->focal_body);
        }
        else if (key == RIGHT_ARROW)
        {
            game_actions_move_rocket(0, boost, aux->focal_body);
        }
        else if (key == DOWN_ARROW)
        {
            game_actions_move_rocket(M_PI * 3.0 / 2, boost, aux->focal_body);
        }
        else if (key == UP_ARROW)
        {
            game_actions_move_rocket(M_PI * 1.0 / 2, boost, aux->focal_body);
        }
        else if (key == A_KEY)
        {
            aux->game_state_num = A_KEY_VALUE;
        }
        else if (key == Q_KEY)
        {
            aux->game_state_num = Q_KEY_VALUE;
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

bool game_actions_restart_game(body_t *focal_body)
{
    vector_t pos = body_get_centroid(focal_body);
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

game_state_t *game_actions_game_restart_aux(game_state_t *aux, body_t *focal_body)
{
    aux->game_state_num = GA_STARTING_KEY_VALUE;
    aux->focal_body = focal_body;
    return aux;
}