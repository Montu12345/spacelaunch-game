#include "damping.h"
#include "body.h"
#include "color.h"
#include "forces.h"
#include "list.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "stdio.h"

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;

const int BALL_RADIUS = 20;
const int ANCHOR_RADIUS = BALL_RADIUS / 2;
const int BALL_MASS = 10;
const int ANCHOR_MASS = INFINITY;
const int HOOKES_CONSTANT = 20;
const int MAX_Y_IMPULSE = 5000;

const vector_t MIN = {.x = 0, .y = 0};
const vector_t MAX = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};
const rgb_color_t ANCHOR_COLOR = {.r = 1, .g = 1, .b = 1};

size_t num_balls() { return SCREEN_SIZE_X / (BALL_RADIUS * 2); }

void make_anchor(scene_t *scene, int idx) {
  list_t *anchor = sprite_make_circle(ANCHOR_RADIUS);
  body_t *anchor_body = body_init(anchor, ANCHOR_MASS, ANCHOR_COLOR);
  vector_t anchor_pos = {.x = BALL_RADIUS * 2 * idx + BALL_RADIUS,
                         .y = SCREEN_SIZE_Y / 2};
  vector_t initial_velocity = VEC_ZERO;
  body_set_centroid(anchor_body, anchor_pos);
  scene_add_body(scene, anchor_body);
  body_set_velocity(anchor_body, initial_velocity);
}

void make_bodies(scene_t *scene) {
  for (size_t i = 0; i < num_balls(); i++) {
    make_anchor(scene, i);
    list_t *circle = sprite_make_circle(BALL_RADIUS);
    rgb_color_t circle_color = rgb_init_random_bright();
    body_t *body = body_init(circle, BALL_MASS, circle_color);
    vector_t position = {.x = BALL_RADIUS * 2 * i + BALL_RADIUS,
                         .y = SCREEN_SIZE_Y / 2};
    body_set_centroid(body, position);
    scene_add_body(scene, body);
    body_set_velocity(body, VEC_ZERO);
  }
}

void move_bodies(scene_t *scene) {
  size_t number_of_balls = num_balls();
  for (size_t i = 0; i < 2 * number_of_balls - 1; i = i + 2) {
    create_spring(scene, HOOKES_CONSTANT, scene_get_body(scene, i),
                  scene_get_body(scene, i + 1));
    vector_t impulse_value = {.x = 0, .y = MAX_Y_IMPULSE * 10 / (i + 10)};
    body_add_impulse(scene_get_body(scene, i + 1), impulse_value);
  }
}

int main() {
  scene_t *scene = scene_init();
  make_bodies(scene);
  sdl_init(MIN, MAX);
  move_bodies(scene);
  while (!sdl_is_done()) {
    double dt = time_since_last_tick();
    scene_tick(scene, dt);
    sdl_clear();
    sdl_render_scene(scene);
    sdl_show();
  }
  scene_free(scene);
}