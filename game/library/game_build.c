#include "game_build.h"

const int GB_SCREEN_SIZE_X = 1000;
const int GB_SCREEN_SIZE_Y = 500;

const int GB_MAX_OBSTACLES_SCREEN_SIZE_X = 2000;
const int GB_MAX_OBSTACLES_SCREEN_SIZE_Y = 1000;

const double GB_ASTEROID_MASS = INFINITY;
const rgb_color_t GB_BAD_ASTEROID_COLOR = {.r = 0, .g = 0, .b = 0};
const rgb_color_t GB_GOOD_ASTEROID_COLOR = {.r = 0, .g = 1, .b = 0};
const int GB_INITIAL_ASTEROIDS = 5;
const int GB_ASTEROID_RADIUS = 50;

const rgb_color_t GB_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = .55};

const int GB_DISTANCE_BETWEEN_STARS = 125;
const double GB_FREQUENCY_FOR_STARS = 50.0;
const rgb_color_t GB_STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const int GB_STAR_NUM_OF_POINTS = 4;
const int GB_STAR_MIN_LENGTH = 2;
const int GB_STAR_MAX_LENGTH = 4;

const vector_t GB_ROCKET_INITIAL_POS = {.x = GB_SCREEN_SIZE_X / 10, .y = GB_SCREEN_SIZE_Y / 10};
const int GB_ROCKET_RADIUS = 30;
const double GB_ROCKET_MASS = 100;
const rgb_color_t GB_ROCKET_COLOR = {.r = 0, .g = 0, .b = 0};

const int GB_SHOOTING_STAR_RADIUS = 3;
const int GB_SHOOTING_STAR_MASS = INFINITY;
const rgb_color_t GB_SHOOTING_STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const vector_t GB_SHOOTING_STAR_VELCOITY = {.x = 700, .y = 0};

enum space_body_type_t
{
    GOOD_OBSTACLE,
    BAD_OBSTACLE,
    ROCKET,
    BACKGROUND_OBJECT,
    STAR,
    SHOOTING_STAR,
};

enum space_body_type_t *space_body_type_init(enum space_body_type_t b)
{
    enum space_body_type_t *body_type = malloc(sizeof(enum space_body_type_t));
    *body_type = b;
    return body_type;
}

enum space_body_type_t *game_build_body_type_init(enum space_body_type_t b)
{
    enum space_body_type_t *body_type = malloc(sizeof(enum space_body_type_t));
    *body_type = b;
    return body_type;
}

void game_build_shooting_star(scene_t *scene)
{
    list_t *shooting_star_list = sprite_make_circle(GB_SHOOTING_STAR_RADIUS);
    body_t *shooting_star = body_init_with_info(shooting_star_list,
                                                GB_SHOOTING_STAR_MASS,
                                                GB_SHOOTING_STAR_COLOR,
                                                game_build_body_type_init(STAR),
                                                free);
    vector_t pos = {.x = 0, .y = rand() % GB_MAX_OBSTACLES_SCREEN_SIZE_X};
    vector_t velocity = GB_SHOOTING_STAR_VELCOITY;
    body_set_velocity(shooting_star, velocity);
    body_set_centroid(shooting_star, pos);
    scene_add_body(scene, shooting_star);
}

void game_build_draw_asteroids(scene_t *scene, body_t *rocket)
{
    for (int i = 0; i < GB_INITIAL_ASTEROIDS; i++)
    {
        game_build_asteroid(scene, rocket);
    }
}

void game_build_draw_stary_night(scene_t *scene)
{
    game_build_sky(scene);
    game_build_stars(scene);
}

body_t *game_build_rocket(scene_t *scene)
{
    list_t *rocket_shape = sprite_make_pacman(GB_ROCKET_RADIUS);
    body_t *rocket = body_init_with_info(rocket_shape,
                                         GB_ROCKET_MASS,
                                         GB_ROCKET_COLOR,
                                         game_build_body_type_init(ROCKET),
                                         free);
    body_set_centroid(rocket, GB_ROCKET_INITIAL_POS);
    body_set_movable(rocket, true);
    scene_add_body(scene, rocket);
    return rocket;
}

void game_build_sky(scene_t *scene)
{
    list_t *background_list = sprite_make_rect(0,
                                               GB_MAX_OBSTACLES_SCREEN_SIZE_X,
                                               0,
                                               GB_MAX_OBSTACLES_SCREEN_SIZE_Y);
    body_t *background = body_init_with_info(background_list,
                                             INFINITY,
                                             GB_BACKGROUND_COLOR,
                                             game_build_body_type_init(BACKGROUND_OBJECT),
                                             free);
    scene_add_body(scene, background);
}

void game_build_stars(scene_t *scene)
{

    for (int i = 0; i < GB_MAX_OBSTACLES_SCREEN_SIZE_Y / GB_FREQUENCY_FOR_STARS; i++)
    {
        for (int j = 0; j < GB_MAX_OBSTACLES_SCREEN_SIZE_X / GB_FREQUENCY_FOR_STARS; j++)
        {
            list_t *star_list = sprite_make_star(GB_STAR_NUM_OF_POINTS,
                                                 GB_STAR_MIN_LENGTH,
                                                 GB_STAR_MAX_LENGTH);
            body_t *star = body_init_with_info(star_list,
                                               INFINITY,
                                               GB_STAR_COLOR,
                                               game_build_body_type_init(STAR),
                                               free);
            vector_t pos = {.x = i * GB_DISTANCE_BETWEEN_STARS,
                            .y = j * GB_DISTANCE_BETWEEN_STARS + (i % 2) * GB_DISTANCE_BETWEEN_STARS / 2.0};
            body_set_centroid(star, pos);
            scene_add_body(scene, star);
        }
    }
}

void game_build_asteroid(scene_t *scene, body_t *rocket)
{
    list_t *circle = sprite_make_circle(GB_ASTEROID_RADIUS);
    rgb_color_t color;
    enum space_body_type_t *obstacle_type;
    if (rand() % 2 == 0)
    {
        color = GB_BAD_ASTEROID_COLOR;
        obstacle_type = game_build_body_type_init(GOOD_OBSTACLE);
    }
    else
    {
        color = GB_GOOD_ASTEROID_COLOR;
        obstacle_type = game_build_body_type_init(BAD_OBSTACLE);
    }
    body_t *asteroid = body_init_with_info(circle, GB_ASTEROID_MASS, color, obstacle_type, free);
    vector_t position = {.x = (rand() % GB_SCREEN_SIZE_X), .y = rand() % GB_SCREEN_SIZE_Y};
    body_set_centroid(asteroid, position);
    body_set_movable(asteroid, false);
    game_actions_rocket_obstacles_collision(scene, rocket, asteroid);
    scene_add_body(scene, asteroid);
}