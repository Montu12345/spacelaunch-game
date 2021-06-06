#include "game_build.h"

const int GB_SCREEN_SIZE_X = 1000;
const int GB_SCREEN_SIZE_Y = 500;

const vector_t GB_min = {.x = 0, .y = 0};
const vector_t GB_max = {.x = GB_SCREEN_SIZE_X, .y = GB_SCREEN_SIZE_Y};

const vector_t ARENA_MIN = {.x = 0, .y = 0};
const vector_t ARENA_MAX = {.x = 2000, .y = 1000};
const vector_t GB_ROCKET_INITIAL_POS = {.x = 50, .y = 500};

const double GB_ASTEROID_MASS = INFINITY;
const rgb_color_t GB_BAD_ASTEROID_COLOR = {.r = 0, .g = 0, .b = 0};
const rgb_color_t GB_GOOD_ASTEROID_COLOR = {.r = 0, .g = 1, .b = 0};
const int GB_INITIAL_ASTEROIDS = 5;
const int GB_ASTEROID_RADIUS = 50;
const int GB_ASTEROID_CONTAINER_LENGTH = 150;
char *GB_GOOD_ASTEROID_TEXTURE = "game/textures/good_asteroid.png";
char *GB_BAD_ASTEROID_TEXTURE = "game/textures/bad_asteroid.png";
char *GB_WELCOME_TEXTURE = "game/textures/welcome.png";
char *GB_HELP_TEXTURE = "game/textures/help.png";
char *GB_LOST_TEXTURE = "game/textures/lost.png";
char *GB_WON_TEXTURE = "game/textures/won.png";
const int GB_ASTEROIDS_PER_LEVEL = 10;

const rgb_color_t GB_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = .55};

const int GB_DISTANCE_BETWEEN_STARS = 125;
const double GB_FREQUENCY_FOR_STARS = 50.0;
const rgb_color_t GB_STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const int GB_STAR_NUM_OF_POINTS = 4;
const int GB_STAR_MIN_LENGTH = 2;
const int GB_STAR_MAX_LENGTH = 4;

const int GB_ROCKET_RADIUS = 30;
const double GB_ROCKET_MASS = 100;
const rgb_color_t GB_ROCKET_COLOR = {.r = 0, .g = 0, .b = 0};

const int GB_SHOOTING_STAR_RADIUS = 3;
const int GB_SHOOTING_STAR_MASS = INFINITY;
const rgb_color_t GB_SHOOTING_STAR_COLOR = {.r = 1, .g = 1, .b = 1};
const vector_t GB_SHOOTING_STAR_VELCOITY = {.x = 700, .y = 0};

const rgb_color_t SCORE_DISPLAY_COLOR = {.r = 1, .g = 0, .b = 0};
const vector_t SCORE_DISPLAY_LEFT = {.x = 40, .y = GB_SCREEN_SIZE_Y - 25};

const int ROCKET_TEXTURE_COUNT = 4;

const int GB_TEXT_WIDTH = 100;
const int GB_TEXT_HEIGHT = 30;

const vector_t GB_SCORE_POSITION = {
    .x = GB_SCREEN_SIZE_X / 2.0 - GB_TEXT_WIDTH / 2.0, .y = 5};
const vector_t GB_SCORE_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_TIMER_POSITION = {.x = GB_SCREEN_SIZE_X - GB_TEXT_WIDTH - 15,
                                    .y = 5};
const vector_t GB_TIMER_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_HELP_POSITION = {.x = 10, .y = GB_SCREEN_SIZE_Y - 50};
const vector_t GB_HELP_DIMENSIONS = {.x = GB_TEXT_WIDTH + 50, .y = GB_TEXT_HEIGHT};
const int GB_HELP_SIZE = 200;

const vector_t GB_HEALTH_POSITION = {.x = 40, .y = 35};
const vector_t GB_HEALTH_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_FINAL_SCORE_POS = {
    .x = GB_SCREEN_SIZE_X / 2.0 - GB_TEXT_WIDTH / 2.0,
    .y = GB_SCREEN_SIZE_Y / 2.0 - GB_TEXT_HEIGHT / 2.0};
const vector_t GB_FINAL_SCORE_DIM = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const vector_t GB_LEVEL_POSITION = {
    .x = GB_SCREEN_SIZE_X / 2.0 - GB_TEXT_WIDTH / 2.0,
    .y = GB_SCREEN_SIZE_Y - GB_TEXT_HEIGHT - 15};
const vector_t GB_LEVEL_DIMENSIONS = {.x = GB_TEXT_WIDTH, .y = GB_TEXT_HEIGHT};

const double GB_FENCE_DEPTH = 50;
const rgb_color_t FENCE_COLOR = {.r = 0.1, .g = 0.1, .b = 0.1};

const int BEGINNING_GAME_WELCOME_SIZE = 30;
const vector_t BEGINNING_GAME_WELCOME_DIMENTIONS = {.x = 700, .y = 50};
const vector_t BEGINNING_GAME_WELCOME_POSITION = {
    .x = GB_SCREEN_SIZE_X / 2.0 - 700 / 2.0, .y = GB_SCREEN_SIZE_Y / 2.0 - 250};

const int BEGINNING_GAME_SIZE = 15;
const vector_t BEGINNING_GAME_DIMENTIONS = {.x = 700, .y = 50};
const vector_t BEGINNING_GAME_POSITION = {
    .x = 0 + 10, .y = GB_SCREEN_SIZE_Y / 2.0 - 250};

const rgb_color_t GB_RED = {.r = 1, .g = 0, .b = 0};

const int GB_MAX_THRUST_TICKS = 10;

enum space_body_type_t {
  GOOD_OBSTACLE,
  BAD_OBSTACLE,
  ROCKET,
  BACKGROUND_OBJECT,
  STAR,
  SHOOTING_STAR,
  SCORE_DISPLAY,
  FENCE,
  ENDZONE,
  HELP_DISPLAY,
};

enum space_body_type_t *space_body_type_init(enum space_body_type_t b) {
  enum space_body_type_t *body_type = malloc(sizeof(enum space_body_type_t));
  *body_type = b;
  return body_type;
}

enum space_body_type_t *game_build_body_type_init(enum space_body_type_t b) {
  enum space_body_type_t *body_type = malloc(sizeof(enum space_body_type_t));
  *body_type = b;
  return body_type;
}

void game_build_shooting_star(scene_t *scene) {
  list_t *shooting_star_list = sprite_make_circle(GB_SHOOTING_STAR_RADIUS);
  body_t *shooting_star = body_init_with_info(
      shooting_star_list, GB_SHOOTING_STAR_MASS, GB_SHOOTING_STAR_COLOR,
      game_build_body_type_init(STAR), free);
  vector_t pos = {.x = 0, .y = rand() % (int)ARENA_MAX.x};
  vector_t velocity = GB_SHOOTING_STAR_VELCOITY;
  body_set_velocity(shooting_star, velocity);
  body_set_centroid(shooting_star, pos);
  scene_add_body(scene, shooting_star);
}

void game_build_draw_stary_night(scene_t *scene) {
  game_build_sky(scene);
  game_build_stars(scene);
}

char *rocket_resource_path(game_state_t *state) {
  char *idle_texture = "game/textures/rocket/rocket_idle.png";

  // If the body does not yet exist or it is not moving, return the idle image.
  if (!state->rocket || state->thrust_ticks_remaining == 0) {
    return idle_texture;
  }

  // Rocket is moving, get animation frame
  char *format = "game/textures/rocket/rocket%d.png";
  int texture_idx = 1 + (state->ticks % ROCKET_TEXTURE_COUNT);
  char *path = malloc(strlen(format) * sizeof(char));
  sprintf(path, format, texture_idx);
  return path;
}

body_t *game_build_rocket(scene_t *scene, game_state_t *state) {
  list_t *rocket_shape = sprite_make_circle(GB_ROCKET_RADIUS);
  body_t *rocket =
      body_init_with_info(rocket_shape, GB_ROCKET_MASS, GB_ROCKET_COLOR,
                          game_build_body_type_init(ROCKET), free);
  body_set_centroid(rocket, GB_ROCKET_INITIAL_POS);
  body_set_movable(rocket, true);
  body_set_texture_path_func(rocket, (texture_path_func_t)rocket_resource_path,
                             state, NULL);
  scene_add_body(scene, rocket);
  body_set_camera_mode(rocket, FOLLOW);
  return rocket;
}

void game_build_sky(scene_t *scene) {
  list_t *background_list = sprite_make_rect(
      (int)ARENA_MIN.x, (int)ARENA_MAX.x, (int)ARENA_MIN.y, (int)ARENA_MAX.y);
  body_t *background =
      body_init_with_info(background_list, INFINITY, GB_BACKGROUND_COLOR,
                          game_build_body_type_init(BACKGROUND_OBJECT), free);
  scene_add_body(scene, background);
}

void game_build_stars(scene_t *scene) {

  for (double i = ARENA_MIN.y; i < ARENA_MAX.y / GB_FREQUENCY_FOR_STARS; i++) {
    for (double j = ARENA_MIN.x; j < ARENA_MAX.x / GB_FREQUENCY_FOR_STARS;
         j++) {
      list_t *star_list = sprite_make_star(
          GB_STAR_NUM_OF_POINTS, GB_STAR_MIN_LENGTH, GB_STAR_MAX_LENGTH);
      body_t *star = body_init_with_info(star_list, INFINITY, GB_STAR_COLOR,
                                         game_build_body_type_init(STAR), free);
      vector_t pos = {.x = i * GB_DISTANCE_BETWEEN_STARS,
                      .y = j * GB_DISTANCE_BETWEEN_STARS +
                           ((int)i % 2) * GB_DISTANCE_BETWEEN_STARS / 2.0};
      body_set_centroid(star, pos);
      body_set_camera_mode(star, SCENE);
      scene_add_body(scene, star);
    }
  }
}

double get_rand() { 
  return (double)rand() / (double)RAND_MAX; 
}

void game_build_draw_asteroids(game_state_t *state) {

  double arena_width = ARENA_MAX.x - ARENA_MIN.x;
  double arena_height = ARENA_MAX.y - ARENA_MIN.y;

  int squares_x = arena_width / GB_ASTEROID_CONTAINER_LENGTH;
  int squares_y = arena_height / GB_ASTEROID_CONTAINER_LENGTH;
  int total_squares = squares_x * squares_y;

  // Generated Monte Carlo-style. Number of asteroids is, on average,
  // total_astroids.
  int total_asteroids = GB_ASTEROIDS_PER_LEVEL * state->level;
  double density = total_asteroids / (double)total_squares;

  // amount on either side of perfect center the asteroid can move
  double render_range = GB_ASTEROID_CONTAINER_LENGTH - 2 * GB_ASTEROID_RADIUS;

  for (int x = 0; x < squares_x; x++) {
    for (int y = 0; y < squares_y; y++) {

      if (get_rand() < density) {
        double x_offset = get_rand() * render_range;
        double y_offset = get_rand() * render_range;
        vector_t centroid = {.x = x_offset + (x * GB_ASTEROID_CONTAINER_LENGTH),
                             .y =
                                 y_offset + (y * GB_ASTEROID_CONTAINER_LENGTH)};

        // printf("squares %f %f\n", centroid.x, centroid.y);
        game_build_asteroid(state, centroid);
      }
    }
  }
}

void game_build_endzone(game_state_t *state) {
  double arena_height = ARENA_MAX.y - ARENA_MIN.y;
  list_t *shape = sprite_make_rect(0, GB_FENCE_DEPTH, ARENA_MIN.y,
                                   ARENA_MAX.y + GB_FENCE_DEPTH);
  vector_t centroid = {.x = ARENA_MAX.x - (GB_FENCE_DEPTH / 2),
                       .y = ARENA_MIN.y - (GB_FENCE_DEPTH / 2) +
                            arena_height / 2.0};
  enum space_body_type_t *obstacle_type = game_build_body_type_init(ENDZONE);
  body_t *endzone = body_init_with_info(shape, INFINITY,
                                        (rgb_color_t){.r = 1, .g = 1, .b = 1},
                                        obstacle_type, free);
  body_set_centroid(endzone, centroid);
  body_set_movable(endzone, false);
  game_actions_rocket_endzone_collision(state, endzone);
  body_set_camera_mode(endzone, SCENE);
  scene_add_body(state->scene, endzone);
}

void add_fence_to_scene(game_state_t *state, list_t *shape, vector_t centroid) {
  enum space_body_type_t *obstacle_type = game_build_body_type_init(FENCE);
  body_t *fence =
      body_init_with_info(shape, INFINITY, FENCE_COLOR, obstacle_type, free);
  body_set_centroid(fence, centroid);
  body_set_movable(fence, false);
  game_actions_rocket_fence_collision(state, fence);
  body_set_camera_mode(fence, SCENE);
  scene_add_body(state->scene, fence);
}

void game_build_fence(game_state_t *state) {
  double arena_width = ARENA_MAX.x - ARENA_MIN.x;
  double arena_height = ARENA_MAX.y - ARENA_MIN.y;

  list_t *bottom_shape =
      sprite_make_rect(ARENA_MIN.x, ARENA_MAX.x, 0, GB_FENCE_DEPTH);
  vector_t bottom_centroid = {.x = ARENA_MIN.x + arena_width / 2.0,
                              .y = -GB_FENCE_DEPTH / 2};
  add_fence_to_scene(state, bottom_shape, bottom_centroid);

  list_t *top_shape =
      sprite_make_rect(ARENA_MIN.x, ARENA_MAX.x, 0, GB_FENCE_DEPTH);
  vector_t top_centroid = {.x = ARENA_MIN.x + arena_width / 2.0,
                           .y = ARENA_MAX.y - (GB_FENCE_DEPTH / 2)};
  add_fence_to_scene(state, top_shape, top_centroid);

  list_t *left_shape = sprite_make_rect(0, GB_FENCE_DEPTH, ARENA_MIN.y,
                                        ARENA_MAX.y + GB_FENCE_DEPTH);
  vector_t left_centroid = {.x = ARENA_MIN.x - (GB_FENCE_DEPTH / 2),
                            .y = ARENA_MIN.y - (GB_FENCE_DEPTH / 2) +
                                 arena_height / 2.0};

  add_fence_to_scene(state, left_shape, left_centroid);
}

void game_build_asteroid(game_state_t *state, vector_t centroid) {
  list_t *circle = sprite_make_circle(GB_ASTEROID_RADIUS);
  bool is_good_asteroid = rand() % 2 == 0;
  char *texture_path = GB_GOOD_ASTEROID_TEXTURE;
  rgb_color_t color;
  enum space_body_type_t *obstacle_type;
  if (is_good_asteroid) {
    color = GB_BAD_ASTEROID_COLOR;
    obstacle_type = game_build_body_type_init(GOOD_OBSTACLE);
  } else {
    texture_path = GB_BAD_ASTEROID_TEXTURE;
    color = GB_GOOD_ASTEROID_COLOR;
    obstacle_type = game_build_body_type_init(BAD_OBSTACLE);
  }
  body_t *asteroid =
      body_init_with_info(circle, GB_ASTEROID_MASS, color, obstacle_type, free);
  body_set_static_texture_path(asteroid, texture_path);
  body_set_centroid(asteroid, centroid);
  body_set_movable(asteroid, false);
  game_actions_rocket_obstacles_collision(state->scene, state->rocket, asteroid,
                                          state);
  body_set_camera_mode(asteroid, SCENE);
  scene_add_body(state->scene, asteroid);
}

body_t *game_build_score_keeper(scene_t *scene, double width, double height) {
  list_t *score_display_list = sprite_make_rect(0, width, 0, height);
  body_t *score_display =
      body_init_with_info(score_display_list, INFINITY, SCORE_DISPLAY_COLOR,
                          game_build_body_type_init(SCORE_DISPLAY), free);

  vector_t score_centroid = SCORE_DISPLAY_LEFT;
  score_centroid.x += width / 2;
  body_set_centroid(score_display, score_centroid);
  body_set_movable(score_display, false);
  scene_add_body(scene, score_display);
  return score_display;
}

void game_build_display_score(game_state_t *state) {
  state->texts->score =
      text_init("SCORE: ", GB_SCORE_POSITION, GB_TEXT_WIDTH,
                        state->score, GB_SCORE_DIMENSIONS);
  scene_add_text(state->scene, state->texts->score);
}

void game_build_display_timer(game_state_t *state) {
  state->texts->timer =
      text_init("TIMER: ", GB_TIMER_POSITION, GB_TEXT_WIDTH,
                        state->timer, GB_TIMER_DIMENSIONS);
  scene_add_text(state->scene, state->texts->timer);
}

void game_build_display_health(game_state_t *state) {
  state->texts->health =
      text_init("HEALTH: ", GB_HEALTH_POSITION, GB_TEXT_WIDTH,
                        state->health, GB_HEALTH_DIMENSIONS);
  scene_add_text(state->scene, state->texts->health);
}

void game_build_display_level(game_state_t *state) {
  state->texts->level =
      text_init("LEVEL: ", GB_LEVEL_POSITION, GB_TEXT_WIDTH,
                        state->level, GB_LEVEL_DIMENSIONS);
  scene_add_text(state->scene, state->texts->level);
}

void game_build_display_text(game_state_t *state) {
  game_build_display_score(state);
  game_build_display_timer(state);
  game_build_display_health(state);
  game_build_display_level(state);
}

void game_update_texts(game_state_t *state) {
  text_set_numbers(state->texts->score, state->score);
  text_set_numbers(state->texts->timer, state->timer);
  text_set_numbers(state->texts->health, state->health);
  text_set_numbers(state->texts->level, state->level);
}

void game_build_help(game_state_t *state) {
  text_t *help = text_init("PRESS H FOR HELP", GB_HELP_POSITION,
                                 GB_HELP_SIZE, -10, GB_HELP_DIMENSIONS);
  scene_add_text(state->scene, help);
}

void game_build_blue_back(game_state_t *state){
  list_t *screen_rect =
        sprite_make_rect(GB_min.x, GB_max.x, GB_min.y, GB_max.y);
    body_t *background = body_init_with_info(screen_rect, 0, GB_BACKGROUND_COLOR, game_build_body_type_init(HELP_DISPLAY), free);
    scene_add_body(state->scene, background);
}

void game_build_help_screen(game_state_t *state){
  list_t *rectangle = sprite_make_rect(0, GB_SCREEN_SIZE_X, 0, GB_SCREEN_SIZE_Y);
  body_t *help = body_init_with_info(rectangle, GB_ASTEROID_MASS, GB_RED, game_build_body_type_init(HELP_DISPLAY), free);
  body_set_static_texture_path(help, GB_HELP_TEXTURE);
  vector_t centroid = {.x = GB_SCREEN_SIZE_X / 2.0, .y = GB_SCREEN_SIZE_Y / 2.0};
  body_set_centroid(help, centroid);
  body_set_movable(help, false);
  scene_add_body(state->scene, help);
  state->needs_restart = false;
}

void game_build_stationary_rockets(game_state_t *state){
  body_t *rocket = game_build_rocket(state->scene, state);
  vector_t rocket_position = {.x = 130, .y = GB_SCREEN_SIZE_Y - 65};
  body_set_centroid(rocket, rocket_position);
  body_set_rotation(rocket, M_PI * 1.0 / 2);
  body_set_type(rocket, game_build_body_type_init(HELP_DISPLAY));
  state->thrust_ticks_remaining = GB_MAX_THRUST_TICKS;
  body_t *rocket_2 = game_build_rocket(state->scene, state);
  vector_t rocket_2_position = {.x = GB_SCREEN_SIZE_X - 130, .y = GB_SCREEN_SIZE_Y - 65};
  body_set_centroid(rocket_2, rocket_2_position);
  body_set_rotation(rocket_2, M_PI * 1.0 / 2);
  body_set_type(rocket_2, game_build_body_type_init(HELP_DISPLAY));
}

void game_build_welcome_background(game_state_t *state){
  list_t *rectangle = sprite_make_rect(0, GB_SCREEN_SIZE_X, 0, GB_SCREEN_SIZE_Y);
  body_t *welcome = body_init(rectangle, GB_ASTEROID_MASS, GB_RED);
  body_set_static_texture_path(welcome, GB_WELCOME_TEXTURE);
  vector_t centroid = {.x = GB_SCREEN_SIZE_X / 2.0, .y = GB_SCREEN_SIZE_Y / 2.0};
  body_set_centroid(welcome, centroid);
  body_set_movable(welcome, false);
  scene_add_body(state->scene, welcome);
}

void game_build_lost_background(game_state_t *state){
  list_t *rectangle = sprite_make_rect(0, GB_SCREEN_SIZE_X, 0, GB_SCREEN_SIZE_Y);
  body_t *lost = body_init(rectangle, GB_ASTEROID_MASS, GB_RED);
  body_set_static_texture_path(lost, GB_LOST_TEXTURE);
  vector_t centroid = {.x = GB_SCREEN_SIZE_X / 2.0, .y = GB_SCREEN_SIZE_Y / 2.0};
  body_set_centroid(lost, centroid);
  body_set_movable(lost, false);
  scene_add_body(state->scene, lost);
}

void game_build_won_background(game_state_t *state){
  list_t *rectangle = sprite_make_rect(0, GB_SCREEN_SIZE_X, 0, GB_SCREEN_SIZE_Y);
  body_t *won = body_init(rectangle, GB_ASTEROID_MASS, GB_RED);
  body_set_static_texture_path(won, GB_WON_TEXTURE);
  vector_t centroid = {.x = GB_SCREEN_SIZE_X / 2.0, .y = GB_SCREEN_SIZE_Y / 2.0};
  body_set_centroid(won, centroid);
  body_set_movable(won, false);
  scene_add_body(state->scene, won);
}