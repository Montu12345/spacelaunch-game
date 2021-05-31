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

char *GB_GOOD_ASTEROID_TEXTURE = "game/textures/good_asteroid.png";
char *GB_BAD_ASTEROID_TEXTURE = "game/textures/bad_asteroid.png";

// const int SCORE_DISPLAY_HIGHT = 20;
// const int SCORE_DISPLAY_WIDTH = 150;
// const vector_t SCORE_DISPLAY_POSITION = {.x = 90, .y = GB_SCREEN_SIZE_Y - 25};
const rgb_color_t SCORE_DISPLAY_COLOR = {.r = 0, .g = 1, .b = 1};

const int ROCKET_TEXTURE_COUNT = 4;

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

//changed
void game_build_draw_asteroids(game_state_t *state, body_t *rocket)
{
    for (int i = 0; i < GB_INITIAL_ASTEROIDS; i++)
    {
        game_build_asteroid(state, rocket);
    }
}

void game_build_draw_stary_night(scene_t *scene)
{
    game_build_sky(scene);
    game_build_stars(scene);
}

char *rocket_resource_path(game_state_t *state)
{
    char *idle_texture = "game/textures/rocket/rocket_idle.png";

    // If the body does not yet exist or it is not moving, return the idle image.
    if (!state->rocket || !body_has_impulse(state->rocket))
    {
        return idle_texture;
    }

    // Rocket is moving, get animation frame
    char *format = "game/textures/rocket/rocket%d.png";
    int texture_idx = 1 + (state->ticks % ROCKET_TEXTURE_COUNT);
    char *path = malloc(strlen(format) * sizeof(char));
    sprintf(path, format, texture_idx);
    return path;
}

body_t *game_build_rocket(scene_t *scene, game_state_t *state)
{
    list_t *rocket_shape = sprite_make_circle(GB_ROCKET_RADIUS);
    body_t *rocket = body_init_with_info(rocket_shape,
                                         GB_ROCKET_MASS,
                                         GB_ROCKET_COLOR,
                                         game_build_body_type_init(ROCKET),
                                         free);
    body_set_centroid(rocket, GB_ROCKET_INITIAL_POS);
    body_set_movable(rocket, true);
    body_set_texture_path_func(rocket, (texture_path_func_t)rocket_resource_path, state, NULL);
    scene_add_body(scene, rocket);
    body_set_camera_mode(rocket, FOLLOW);
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
            body_set_camera_mode(star, SCENE);
            scene_add_body(scene, star);
        }
    }
}

// changed
void game_build_asteroid(game_state_t *state, body_t *rocket)
{
    list_t *circle = sprite_make_circle(GB_ASTEROID_RADIUS);
    bool is_good_asteroid = rand() % 2 == 0;
    char *texture_path = GB_GOOD_ASTEROID_TEXTURE;
    rgb_color_t color;
    enum space_body_type_t *obstacle_type;
    if (is_good_asteroid)
    {
        color = GB_BAD_ASTEROID_COLOR;
        obstacle_type = game_build_body_type_init(GOOD_OBSTACLE);
    }
    else
    {
        texture_path = GB_BAD_ASTEROID_TEXTURE;
        color = GB_GOOD_ASTEROID_COLOR;
        obstacle_type = game_build_body_type_init(BAD_OBSTACLE);
    }
    body_t *asteroid = body_init_with_info(circle, GB_ASTEROID_MASS, color, obstacle_type, free);
    body_set_static_texture_path(asteroid, texture_path);
    vector_t position = {.x = (rand() % GB_SCREEN_SIZE_X), .y = rand() % GB_SCREEN_SIZE_Y};
    body_set_centroid(asteroid, position);
    body_set_movable(asteroid, false);
    game_actions_rocket_obstacles_collision(state->scene, rocket, asteroid, state);
    body_set_camera_mode(asteroid, SCENE);
    scene_add_body(state->scene, asteroid);
}

body_t *game_build_score_keeper(scene_t *scene, double width, double height, vector_t position)
{
    list_t *score_display_list = sprite_make_rect(0, width, 0, height);
    body_t *score_display = body_init_with_info(score_display_list, INFINITY, SCORE_DISPLAY_COLOR, game_build_body_type_init(SCORE_DISPLAY), free);
    body_set_centroid(score_display, position);
    body_set_movable(score_display, false);
    scene_add_body(scene, score_display);
    return score_display;
}

