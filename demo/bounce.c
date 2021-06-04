#include "bounce.h"
#include "sprite.h"
#include <math.h>
#include <stdlib.h>

/** @file bounce.c
 *  @brief Function implementations for a bounce animation
 *
 *  This contains the function implementations for a polygon
 *  bouncing animation.
 *
 *  @author Alexis Wang, Ian Fowler
 *  @bug No known bugs.
 */

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;
const double ANGLE = M_PI;
const int INITIAL_VELOCITY_X = 1000;
const int INITIAL_VELOCITY_Y = 1000;
const double MIN_STAR_LENGTH = 40;
const double MAX_STAR_LENGTH = 100;
const rgb_color_t STAR_COLOR = {
    .r = 205 / 255.0, .g = 92 / 255.0, .b = 92 / 255.0};

void compute_new_positions(list_t *object, double dt, vector_t *velocity) {
  polygon_rotate(object, ANGLE * dt, polygon_centroid(object));
  double x_scale = 1.0;
  double y_scale = 1.0;
  for (size_t i = 0; i < list_size(object); i++) {
    vector_t *vec = list_get((object), i);
    if (velocity->x > 0 && vec->x >= SCREEN_SIZE_X) {
      x_scale = -1.0;
    }
    if (velocity->x < 0 && vec->x <= 0) {
      x_scale = -1.0;
    }
    if (velocity->y > 0 && vec->y >= SCREEN_SIZE_Y) {
      y_scale = -1.0;
    }
    if (velocity->y < 0 && vec->y <= 0) {
      y_scale = -1.0;
    }
  }
  vector_t next_velocity = {.x = x_scale * velocity->x,
                            .y = y_scale * velocity->y};
  *velocity = next_velocity;
  polygon_translate(object, vec_multiply(dt, *velocity));
}

int main(int argc, char *argv[]) {
  // Get screen size using our screen size constants
  vector_t min = {.x = 0, .y = 0};
  vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};
  rgb_color_t color = rgb_init_random_bright();
  list_t *object_pos = sprite_make_star(5, MIN_STAR_LENGTH, MAX_STAR_LENGTH);
  polygon_translate(object_pos, vec_multiply(0.5, max));
  vector_t *velocity = malloc(sizeof(vector_t));
  velocity->x = INITIAL_VELOCITY_X;
  velocity->y = INITIAL_VELOCITY_Y;
  sdl_init(min, max);
  while (!sdl_is_done()) {
    double dt = time_since_last_tick();
    compute_new_positions(object_pos, dt, velocity);
    sdl_clear();
    sdl_draw_polygon(object_pos, color);
    sdl_show();
  }
  list_free(object_pos);
  free(velocity);
}
