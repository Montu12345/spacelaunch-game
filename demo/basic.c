#include "bounce.h"
#include <math.h>
#include <stdlib.h>
#include "sprite.h"

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

const int VEL_X = 1000;
const double MIN_STAR_LENGTH = 40;
const double MAX_STAR_LENGTH = 100;

rgb_color_t COLOR = (rgb_color_t){.r = 1, .g = 0, .b = 0};

void manage_collision(body_t *star)
{
  vector_t centroid = body_get_centroid(star);
  vector_t velocity = body_get_velocity(star);

  if ((centroid.x > SCREEN_SIZE_X && velocity.x > 0) || (centroid.x < 0 && velocity.x < 0))
  {
    body_set_velocity(star, vec_negate(velocity));
  }
}

body_t *spawn_star(scene_t *scene)
{
  list_t *object = sprite_make_star(5, MIN_STAR_LENGTH, MAX_STAR_LENGTH);
  body_t *star = body_init(object, 10, COLOR);
  body_set_velocity(star, (vector_t){.x = VEL_X, .y = 0});
  body_set_centroid(star, vec_multiply(0.5, max));
  scene_add_body(scene, star);
  return star;
}

void move_star(list_t *star, vector_t *velocity, double dt)
{
  vector_t centroid = polygon_centroid(star);

  if ((centroid.x > SCREEN_SIZE_X && velocity->x > 0) || (centroid.x < 0 && velocity->x < 0))
  {
    velocity->x *= -1;
  }
  polygon_translate(star, vec_multiply(dt, *velocity));
}

int main_no_scene()
{
  // Get screen size using our screen size constants
  rgb_color_t color = rgb_init_random_bright();
  list_t *object = sprite_make_star(5, MIN_STAR_LENGTH, MAX_STAR_LENGTH);
  polygon_translate(object, vec_multiply(0.5, max));
  vector_t *velocity = vec_malloc(VEL_X, 0);

  sdl_init(min, max);
  while (!sdl_is_done())
  {
    double dt = time_since_last_tick();
    // delay(30);
    move_star(object, velocity, dt);
    sdl_clear();
    sdl_draw_polygon(object, color);
    sdl_show();
  }
  vec_free(velocity);

  return EXIT_SUCCESS;
}

int main_scene()
{
  // Get screen size using our screen size constants

  scene_t *demo = scene_init();
  body_t *star = spawn_star(demo);

  sdl_init(min, max);
  while (!sdl_is_done())
  {
    double dt = time_since_last_tick();
    scene_tick(demo, dt);
    manage_collision(star);
    sdl_clear();
    sdl_render_scene(demo);
    sdl_show();
  }
  scene_free(demo);
  return EXIT_SUCCESS;
}

int main(int argc, char *argv[])
{
  // main_no_scene();
  main_scene();

  return EXIT_SUCCESS;
}