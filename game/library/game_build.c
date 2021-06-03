#include "game_build.h"

const int GB_SCREEN_SIZE_X = 1000;
const int GB_SCREEN_SIZE_Y = 500;

const vector_t ARENA_MIN = {.x = 0, .y = 0};
const vector_t ARENA_MAX = {.x = 2000, .y = 1000};

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

const rgb_color_t SCORE_DISPLAY_COLOR = {.r = 1, .g = 0, .b = 0};
const vector_t SCORE_DISPLAY_LEFT = {.x = 40, .y = GB_SCREEN_SIZE_Y - 25};

const int ROCKET_TEXTURE_COUNT = 4;

const int GB_TEXT_WIDTH = 100;
const int GB_TEXT_HEIGHT = 30;

const vector_t GB_SCORE_POSITION = {.x = GB_SCREEN_SIZE_X / 2.0 - GB_TEXT_WIDTH / 2.0, .y = 5};
const vector_t GB_SCORE_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_TIMER_POSITION = {.x = GB_SCREEN_SIZE_X - GB_TEXT_WIDTH - 15, .y = 5};
const vector_t GB_TIMER_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_HELP_POSITION = {.x = 10, .y = GB_SCREEN_SIZE_Y - 50};
const vector_t GB_HELP_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};
const int GB_HELP_SIZE = 200;

const vector_t GB_HEALTH_POSITION = {.x = 40, .y = 35};
const vector_t GB_HEALTH_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};


const vector_t GB_FINAL_SCORE_POS = {.x = GB_SCREEN_SIZE_X / 2.0 - GB_TEXT_WIDTH / 2.0, .y = GB_SCREEN_SIZE_Y / 2.0 - GB_TEXT_HEIGHT / 2.0};
const vector_t GB_FINAL_SCORE_DIM = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_LEVEL_POSITION = {.x = GB_SCREEN_SIZE_X / 2.0 - GB_TEXT_WIDTH / 2.0, .y = GB_SCREEN_SIZE_Y - GB_TEXT_HEIGHT - 15};
const vector_t GB_LEVEL_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

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
    vector_t pos = {.x = 0, .y = rand() % (int)ARENA_MAX.x};
    vector_t velocity = GB_SHOOTING_STAR_VELCOITY;
    body_set_velocity(shooting_star, velocity);
    body_set_centroid(shooting_star, pos);
    scene_add_body(scene, shooting_star);
}

//changed
void game_build_draw_asteroids(game_state_t *state, body_t *rocket, vector_t min, vector_t max)
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
    if (!state->rocket || state->thrust_ticks_remaining == 0)
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
    list_t *background_list = sprite_make_rect((int)ARENA_MIN.x,
                                               (int)ARENA_MAX.x,
                                               (int)ARENA_MIN.y,
                                               (int)ARENA_MAX.y);
    body_t *background = body_init_with_info(background_list,
                                             INFINITY,
                                             GB_BACKGROUND_COLOR,
                                             game_build_body_type_init(BACKGROUND_OBJECT),
                                             free);
    scene_add_body(scene, background);
}

void game_build_stars(scene_t *scene)
{

    for (double i = ARENA_MIN.y; i < ARENA_MAX.y / GB_FREQUENCY_FOR_STARS; i++)
    {
        for (double j = ARENA_MIN.x; j < ARENA_MAX.x / GB_FREQUENCY_FOR_STARS; j++)
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
                            .y = j * GB_DISTANCE_BETWEEN_STARS + ((int)i % 2) * GB_DISTANCE_BETWEEN_STARS / 2.0};
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

body_t *game_build_score_keeper(scene_t *scene, double width, double height)
{
    list_t *score_display_list = sprite_make_rect(0, width, 0, height);
    body_t *score_display = body_init_with_info(score_display_list,
                                                INFINITY,
                                                SCORE_DISPLAY_COLOR,
                                                game_build_body_type_init(SCORE_DISPLAY),
                                                free);

    vector_t score_centroid = SCORE_DISPLAY_LEFT;
    score_centroid.x += width / 2;
    body_set_centroid(score_display, score_centroid);
    body_set_movable(score_display, false);
    scene_add_body(scene, score_display);
    return score_display;
}

void game_build_display_score(game_state_t *state)
{
    state->texts->score = text_numbers_init("Score: ", GB_SCORE_POSITION, GB_TEXT_WIDTH, state->score, GB_SCORE_DIMENSIONS);
    scene_add_text(state->scene, state->texts->score);
}

void game_build_display_timer(game_state_t *state)
{
    state->texts->timer = text_numbers_init("Timer: ", GB_TIMER_POSITION, GB_TEXT_WIDTH, state->timer, GB_TIMER_DIMENSIONS);
    scene_add_text(state->scene, state->texts->timer);
}

void game_build_display_health(game_state_t *state)
{
    state->texts->health = text_numbers_init("Health: ", GB_HEALTH_POSITION, GB_TEXT_WIDTH, state->health, GB_HEALTH_DIMENSIONS);
    scene_add_text(state->scene, state->texts->health);
}

void game_build_display_level(game_state_t *state)
{
    state->texts->level = text_numbers_init("Level: ", GB_LEVEL_POSITION, GB_TEXT_WIDTH, state->level, GB_LEVEL_DIMENSIONS);
    scene_add_text(state->scene, state->texts->level);
}

void game_build_display_text(game_state_t *state)
{
    game_build_display_score(state);
    game_build_display_timer(state);
    game_build_display_health(state);
    game_build_display_level(state);
}

void game_update_texts(game_state_t *state)
{
    text_set_numbers(state->texts->score, state->score);
    text_set_numbers(state->texts->timer, state->timer);
    text_set_numbers(state->texts->health, state->health);
    text_set_numbers(state->texts->level, state->level);
}

void game_build_help(game_state_t *state){
    text_t *help = text_words_init("Press H for help", GB_HELP_POSITION, GB_HELP_SIZE, GB_HELP_DIMENSIONS);
    scene_add_text(state->scene, help);
}