#include "bounce.h"
#include <math.h>
#include <stdlib.h>
#include "sprite.h"
#include "collision.h"
#include <stdio.h>
#include "forces.h"
#include <time.h>
#include <unistd.h>
#include "color.h"
#include "breakout.h"

const int SCREEN_SIZE_X = 500;
const int SCREEN_SIZE_Y = 500;
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

const double NUMBER_OF_BOXES_PER_ROW = 10;
const double NUMBER_OF_BOXES_PER_COLUMN = 3;
const double BOXES_RANGE_OF_COLORS = 360;
const int BOXES_COLOR_SATURATION = 1;
const int BOXES_COLOR_HUE = 1;
const int Y_START_OF_BOXES = 10;
const double ROW_SPACE_BETWEEN_BOXES = 5;
const double COLUMN_SPACE_BETWEEN_BOXES = 6;
const int BOX_MASS = INFINITY;

const int PADDLE_MIN_LENGTH = 10;
const int PADDLE_MAX_LENGTH = 50;
const int PADDLE_MASS = INFINITY;
const double PADDLE_X_POS = SCREEN_SIZE_X / 2.0;
const double PADDLE_Y_POS = 20.0;
const vector_t PADDLE_VELOCITY = {.x = 0, .y = 0};

const int BALL_RADIUS = 10;
const double BALL_MASS = 10;
const double BALL_ELASTICITY = 1.08;
const vector_t BALL_VELOCITY = {.x = 100, .y = -200};

const double VERTICAL_SPACE_OF_BOXES = 1 / 10.0 * SCREEN_SIZE_Y;
const double BALL_Y_POS = SCREEN_SIZE_Y - 1 / 5.0 * SCREEN_SIZE_Y - BALL_RADIUS - Y_START_OF_BOXES;

const int PEG_RADIUS = 5;
const int NUMBER_OF_PEGS = 15;
const int PEG_MASS = INFINITY;

const double VELOCITY_BOOST_HOLD_KEY = 10;

void half_destructive_handler(body_t *keep, body_t *erase, vector_t axis, collision_event_aux_t *aux)
{
    vector_t j = impulse_to_body_1(keep, erase, axis, aux->elasticity);
    body_add_impulse(keep, j);
    body_remove(erase);
}

void create_half_desructive_collision(
    scene_t *scene,
    double elasticity,
    body_t *keep,
    body_t *erase)
{
    collision_event_aux_t *aux = malloc(sizeof(collision_event_aux_t));
    aux->elasticity = elasticity;
    create_collision(scene, keep, erase, (collision_handler_t)half_destructive_handler, aux, free);
}

void ball_box_collision(scene_t *scene, body_t *ball, body_t *box)
{
    create_half_desructive_collision(scene, BALL_ELASTICITY, ball, box);
}

void ball_peg_collision(scene_t *scene, body_t *ball, body_t *peg)
{
    create_physics_collision(scene, BALL_ELASTICITY, ball, peg);
}

void ball_paddle_collision(scene_t *scene, body_t *ball, body_t *paddle)
{
    create_physics_collision(scene, BALL_ELASTICITY, ball, paddle);
}

void create_boxes(scene_t *scene, body_t *ball)
{
    double empty_space = ROW_SPACE_BETWEEN_BOXES * (NUMBER_OF_BOXES_PER_ROW + 1);
    double box_width = (SCREEN_SIZE_X - empty_space) / NUMBER_OF_BOXES_PER_ROW;
    double box_height = VERTICAL_SPACE_OF_BOXES * 1 / NUMBER_OF_BOXES_PER_COLUMN;
    for (int i = 0; i < NUMBER_OF_BOXES_PER_COLUMN; i++)
    {
        for (int j = 0; j < NUMBER_OF_BOXES_PER_ROW; j++)
        {
            list_t *box = sprite_make_rect(0, box_width, 0, box_height);
            rgb_color_t color = rgb_from_hsv_init(BOXES_RANGE_OF_COLORS / NUMBER_OF_BOXES_PER_ROW * j,
                                                  BOXES_COLOR_SATURATION,
                                                  BOXES_COLOR_HUE);
            body_t *box_body = body_init(box, BOX_MASS, color);
            double x_pos = ROW_SPACE_BETWEEN_BOXES * (j + 1) + (1.0 / 2 + j) * box_width;
            double y_pos = SCREEN_SIZE_Y - (((1.0 + COLUMN_SPACE_BETWEEN_BOXES) * i + 0.5 * i) / COLUMN_SPACE_BETWEEN_BOXES) * box_height - Y_START_OF_BOXES;
            vector_t position = {.x = x_pos, .y = y_pos};
            body_set_centroid(box_body, position);
            ball_box_collision(scene, ball, box_body);
            scene_add_body(scene, box_body);
        }
    }
}

vector_t wrap_position(vector_t original_position)
{
    if (original_position.x < min.x)
    {
        return (vector_t){.x = max.x - PADDLE_MAX_LENGTH, original_position.y};
    }
    else if (original_position.x > max.x)
    {
        return (vector_t){.x = min.x + PADDLE_MAX_LENGTH, original_position.y};
    }
    else
    {
        return original_position;
    }
}

void move_paddle(double dx, body_t *paddle)
{
    vector_t change_position = (vector_t){dx, 0};
    vector_t paddle_position = body_get_centroid(paddle);
    vector_t next_position = vec_add(paddle_position, change_position);
    vector_t new_position = wrap_position(next_position);
    body_set_centroid(paddle, new_position);
}

void reflect_ball(scene_t *scene, body_t *ball)
{
    vector_t pos = body_get_centroid(ball);
    if (pos.x + BALL_RADIUS > SCREEN_SIZE_X || pos.x - BALL_RADIUS < 0)
    {
        vector_t old_velocity = body_get_velocity(ball);
        vector_t new_velocity = {.x = -old_velocity.x, .y = old_velocity.y};
        body_set_velocity(ball, new_velocity);
    }
}

void handle(char key, key_event_type_t type, double held_time, body_t *paddle)
{
    double pos_scale = held_time + VELOCITY_BOOST_HOLD_KEY;
    if (type == KEY_PRESSED)
    {
        if (key == LEFT_ARROW)
        {
            move_paddle(-pos_scale, paddle);
        }
        else if (key == RIGHT_ARROW)
        {
            move_paddle(pos_scale, paddle);
        }
    }
}

body_t *make_paddle(scene_t *scene)
{
    list_t *paddle = sprite_make_rect(0, PADDLE_MAX_LENGTH, 0, PADDLE_MIN_LENGTH);
    body_t *paddle_body = body_init(paddle, PADDLE_MASS, rgb_init_random_bright());
    vector_t position = {.x = PADDLE_X_POS,
                         .y = PADDLE_Y_POS};
    body_set_centroid(paddle_body, position);
    body_set_velocity(paddle_body, PADDLE_VELOCITY);
    scene_add_body(scene, paddle_body);
    return paddle_body;
}

body_t *create_ball(scene_t *scene)
{
    list_t *ball = sprite_make_circle(BALL_RADIUS);
    body_t *ball_body = body_init(ball, BALL_MASS, rgb_init_random_bright());
    vector_t position = {.x = rand() % (SCREEN_SIZE_X - 30), .y = BALL_Y_POS};
    vector_t velocity = BALL_VELOCITY;
    if (position.x > SCREEN_SIZE_X / 2)
    {
        velocity.x = velocity.x * -1.0;
    }
    body_set_centroid(ball_body, position);
    body_set_velocity(ball_body, velocity);
    scene_add_body(scene, ball_body);
    return ball_body;
}

bool restart_game(body_t *ball)
{
    vector_t pos = body_get_centroid(ball);
    if (pos.y - BALL_RADIUS > SCREEN_SIZE_Y || pos.y - BALL_RADIUS < 0)
    {
        return true;
    }
    return false;
}

void create_pegs(scene_t *scene, body_t *ball)
{
    for (int i = 0; i < NUMBER_OF_PEGS; i++)
    {
        list_t *circle = sprite_make_circle(PEG_RADIUS);
        body_t *peg = body_init(circle, PEG_MASS, rgb_init_random_bright());
        vector_t position = {.x = rand() % SCREEN_SIZE_X, .y = rand() % SCREEN_SIZE_Y};
        if (position.y < PADDLE_Y_POS)
        {
            position.y += PADDLE_Y_POS;
        }
        body_set_centroid(peg, position);
        body_set_movable(peg, false);
        ball_peg_collision(scene, ball, peg);
        scene_add_body(scene, peg);
    }
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

void create_new_game(scene_t *scene, body_t *ball, body_t *paddle)
{
    create_boxes(scene, ball);
    create_pegs(scene, ball);
    ball_paddle_collision(scene, ball, paddle);
}

int main()
{
    srand(time(0));
    scene_t *demo = scene_init();
    body_t *paddle;
    paddle = make_paddle(demo);
    body_t *ball = create_ball(demo);
    create_boxes(demo, ball);
    sdl_init(min, max);
    create_pegs(demo, ball);
    ball_paddle_collision(demo, ball, paddle);
    while (!sdl_is_done())
    {
        sdl_event_args(paddle);
        double dt = time_since_last_tick();
        reflect_ball(demo, ball);
        if (restart_game(ball))
        {
            clear_scene(demo);
            scene_tick(demo, dt);
            ball = create_ball(demo);
            paddle = make_paddle(demo);
            sdl_event_args(paddle);
            create_new_game(demo, ball, paddle);
        }
        scene_tick(demo, dt);
        sdl_on_key((key_handler_t)handle);
        sdl_clear();
        sdl_render_scene(demo);
        sdl_show();
    }
    scene_free(demo);
}
