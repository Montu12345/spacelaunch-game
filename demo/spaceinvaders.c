#include "bounce.h"
#include <math.h>
#include <stdlib.h>
#include "sprite.h"
#include "collision.h"
#include <stdio.h>
#include "forces.h"
#include <time.h>
#include <unistd.h>

const int SCREEN_SIZE_X = 500;
const int SCREEN_SIZE_Y = 500;
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

const int VEL_X = 1000;
const double MIN_STAR_LENGTH = 40;
const double MAX_STAR_LENGTH = 100;
const double INVADER_RADIUS = 30.0;
const double INVADER_BOTTOM_DEPTH = 10.0;
const int DEFENDER_MIN_LENGTH = 10;
const int DEFENDER_MAX_LENGTH = 20;
const int DEFENDER_MASS = 11;
const int INVADER_MASS = 10;
const int BULLET_MASS = 2;
const int BULLET_WIDTH = 5;
const int BULLET_HEIGHT = 10;
const int INVADERS_PER_ROW = 7;
const int INVADERS_PER_COLUMN = 3;
const double DEFENDER_X_POS = SCREEN_SIZE_X / 2.0;
const double DEFENDER_Y_POS = 20.0;
const int FREQUENCY_INVADER_SHOTS = 30;
const double VELOCITY_BOOST_HOLD_KEY = 5;

const rgb_color_t BULLET_COLOR = {.r = 0.5, .g = 0.5, .b = 0.5};

const vector_t INVADERS_VELOCITY = {.x = 100, .y = 0};
const vector_t DEFENDERS_VELOCITY = {.x = 0, .y = 0};
const vector_t DEFENDER_BULLET_VELOCITY = {.x = 0, .y = 500};
const vector_t INVADER_BULLET_VELOCITY = {.x = 0, .y = -500};

enum body_type_t
{
    bullet_t,
    defender_t,
    invader_t
};

typedef struct space_aux
{
    body_t *defender;
    scene_t *demo;
} space_aux_t;

enum body_type_t *body_type_init(enum body_type_t b)
{
    enum body_type_t *body_type = malloc(sizeof(enum body_type_t));
    *body_type = b;
    return body_type;
}

space_aux_t *space_aux_init(body_t *defender, scene_t *demo)
{
    space_aux_t *aux = malloc(sizeof(space_aux_t));
    *aux = (space_aux_t){
        .defender = defender,
        .demo = demo,
    };
    return aux;
}

void bullet_invader_collision(scene_t *scene, body_t *bullets)
{
    for (size_t i = 0; i < scene_bodies(scene); i++)
    {
        body_t *invader = scene_get_body(scene, i);
        if (*(enum body_type_t *)body_get_info(invader) == invader_t)
        {
            create_destructive_collision(scene, invader, bullets);
        }
    }
}

void bullet_defender_collision(scene_t *scene, body_t *bullets, body_t *defender)
{
    body_t *first_body = scene_get_body(scene, 0);
    bool defender_exists = *(enum body_type_t *)body_get_info(first_body) == defender_t;
    if (defender_exists)
    {
        create_destructive_collision(scene, defender, bullets);
    }
}

void invader_defender_collision(scene_t *scene, body_t *invader, body_t *defender)
{
    create_destructive_collision(scene, invader, defender);
}

void make_defender_bullets(scene_t *scene, body_t *defender)
{
    list_t *bullet = sprite_make_rect(0, BULLET_WIDTH, 0, BULLET_HEIGHT);
    body_t *bullet_body = body_init_with_info(bullet, BULLET_MASS, BULLET_COLOR, body_type_init(bullet_t), free);
    body_set_velocity(bullet_body, DEFENDER_BULLET_VELOCITY);
    vector_t defender_cent = body_get_centroid(defender);
    vector_t bullet_position = {.x = defender_cent.x,
                                .y = defender_cent.y + DEFENDER_MIN_LENGTH};
    body_set_centroid(bullet_body, bullet_position);
    scene_add_body(scene, bullet_body);
    bullet_invader_collision(scene, bullet_body);
}

void make_invaders_bullets(scene_t *scene, body_t *invader, body_t *defender)
{
    list_t *bullet = sprite_make_rect(0, BULLET_WIDTH, 0, BULLET_HEIGHT);
    body_t *bullet_body = body_init_with_info(bullet, BULLET_MASS, BULLET_COLOR, body_type_init(bullet_t), free);
    body_set_velocity(bullet_body, INVADER_BULLET_VELOCITY);
    vector_t invader_cent = body_get_centroid(invader);
    vector_t position = {.x = invader_cent.x, .y = invader_cent.y};
    body_set_centroid(bullet_body, position);
    scene_add_body(scene, bullet_body);
    bullet_defender_collision(scene, defender, bullet_body);
}

void make_invaders(scene_t *scene, body_t *defender)
{
    for (int i = 0; i < INVADERS_PER_ROW; i++)
    {
        for (int j = 0; j < INVADERS_PER_COLUMN; j++)
        {
            list_t *invader = sprite_make_invader(INVADER_RADIUS, INVADER_BOTTOM_DEPTH);
            body_t *invader_body = body_init_with_info(invader,
                                                       INVADER_MASS,
                                                       rgb_init_random_bright(),
                                                       body_type_init(invader_t),
                                                       free);
            vector_t position = {.x = SCREEN_SIZE_X * i / (INVADERS_PER_ROW) + INVADER_RADIUS,
                                 .y = SCREEN_SIZE_Y - (INVADER_RADIUS * 2 * (j + .5))};
            body_set_centroid(invader_body, position);
            body_set_velocity(invader_body, INVADERS_VELOCITY);
            scene_add_body(scene, invader_body);
            invader_defender_collision(scene, invader_body, defender);
        }
    }
}

void teleport_invaders(scene_t *scene)
{
    for (size_t i = 0; i < scene_bodies(scene); i = i + 1)
    {
        body_t *invader = scene_get_body(scene, i);
        vector_t centroid = body_get_centroid(invader);
        vector_t velocity = body_get_velocity(invader);
        bool invader_off_right = centroid.x + INVADER_RADIUS > SCREEN_SIZE_X && velocity.x > 0;
        bool invader_off_left = centroid.x - INVADER_RADIUS < 0 && velocity.x < 0;
        if (invader_off_right || invader_off_left)
        {
            vector_t updated_y = {.x = 0,
                                  .y = -(INVADER_RADIUS * 2 * INVADERS_PER_COLUMN)};
            body_set_centroid(invader, vec_add(updated_y, centroid));
            vector_t new_pos = {.x = -velocity.x, .y = 0};
            body_set_velocity(invader, new_pos);
        }
    }
}

vector_t wrap_position(vector_t original_position)
{
    if (original_position.x < min.x)
    {
        return (vector_t){.x = max.x - DEFENDER_MAX_LENGTH, original_position.y};
    }
    else if (original_position.x > max.x)
    {
        return (vector_t){.x = min.x + DEFENDER_MAX_LENGTH, original_position.y};
    }
    else
    {
        return original_position;
    }
}

void move_defender(double dx, body_t *defender)
{
    vector_t change_position = (vector_t){dx, 0};
    vector_t defender_position = body_get_centroid(defender);
    vector_t next_position = vec_add(defender_position, change_position);
    vector_t new_position = wrap_position(next_position);
    body_set_centroid(defender, new_position);
}

void handle(char key, key_event_type_t type, double held_time, space_aux_t *aux)
{
    body_t *first_body = scene_get_body(aux->demo, 0);
    double pos_scale = held_time + VELOCITY_BOOST_HOLD_KEY;
    bool defender_exists = *(enum body_type_t *)body_get_info(first_body) == defender_t;
    if (type == KEY_PRESSED && defender_exists)
    {
        if (key == LEFT_ARROW)
        {
            move_defender(-pos_scale, aux->defender);
        }
        else if (key == RIGHT_ARROW)
        {
            move_defender(pos_scale, aux->defender);
        }
        if (key == SPACEBAR)
        {
            make_defender_bullets(aux->demo, aux->defender);
        }
    }
}

body_t *make_defender(scene_t *scene)
{
    list_t *defender = sprite_make_ellipse(DEFENDER_MAX_LENGTH, DEFENDER_MIN_LENGTH);
    body_t *defender_body = body_init_with_info(defender,
                                                DEFENDER_MASS,
                                                rgb_init_random_bright(),
                                                body_type_init(defender_t),
                                                free);
    vector_t position = {.x = DEFENDER_X_POS,
                         .y = DEFENDER_Y_POS};
    body_set_centroid(defender_body, position);
    body_set_velocity(defender_body, DEFENDERS_VELOCITY);
    scene_add_body(scene, defender_body);
    return defender_body;
}

int invaders_shoot(scene_t *scene, body_t *defender, int i)
{
    if (i == FREQUENCY_INVADER_SHOTS)
    {
        int j = rand() % scene_bodies(scene);
        body_t *invader = scene_get_body(scene, j);
        if (body_get_mass(invader) == INVADER_MASS)
        {
            make_invaders_bullets(scene, invader, defender);
        }
        i = 0;
    }
    return i;
}

int main()
{
    srand(time(0));
    scene_t *demo = scene_init();
    body_t *defender = make_defender(demo);
    space_aux_t *aux = space_aux_init(defender, demo);
    make_invaders(demo, defender);
    int i = 0;
    sdl_init(min, max);
    sdl_event_args(aux);
    while (!sdl_is_done())
    {
        i++;
        double dt = time_since_last_tick();
        teleport_invaders(demo);
        scene_tick(demo, dt);
        i = invaders_shoot(demo, defender, i);
        sdl_clear();
        sdl_on_key((key_handler_t)handle);
        sdl_render_scene(demo);
        sdl_show();
    }
    free(aux);
    scene_free(demo);
}
