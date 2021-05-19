#include "pacman.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/** 
 * Modifications on pacman to test out camera functionality
 */

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;

const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

const int PACMAN_STEP = 10;
const int CIRCLE_PRECISION = 10;
const int PACMAN_PRECISION = 30;
const double PACMAN_VELOCITY_SCALE = 30;
const double EAT_TOLERANCE = 10.0;
const int INITIAL_DOTS = 30;
const double DOT_ADD_PERIOD = 1.0;
const double VELOCITY_BOOST = 1.0;

const rgb_color_t PACMAN_COLOR = {.r = 1, .g = 1, .b = 0};
const rgb_color_t DOT_COLOR = {.r = 1, .g = 1, .b = 0};

vector_t camera_offset_func(body_t *focal_body, void *aux)
{
  vector_t center = vec_multiply(0.5, max);
  return vec_subtract(center, body_get_centroid(focal_body));
}

vector_t camera_mover_func(vector_t offset, body_t *body)
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

vector_t wrap_position(vector_t original_position)
{
  if (original_position.x < min.x)
  {
    return (vector_t){.x = max.x, original_position.y};
  }
  else if (original_position.x > max.x)
  {
    return (vector_t){.x = min.x, original_position.y};
  }
  else if (original_position.y < min.y)
  {
    return (vector_t){.x = original_position.x, max.y};
  }
  else if (original_position.y > max.y)
  {
    return (vector_t){.x = original_position.x, min.y};
  }
  else
  {
    return original_position;
  }
}

bool eat(size_t dot_idx, body_t *dot, body_t *pacman, scene_t *scene)
{
  vector_t p1 = body_get_centroid(pacman);
  vector_t p2 = body_get_centroid(dot);
  double distance = vec_magnitude(vec_subtract(p1, p2));
  if (distance < EAT_TOLERANCE)
  {
    scene_remove_body(scene, dot_idx);
    return true;
  }
  return false;
}

void check_eat(body_t *pacman, scene_t *scene)
{
  for (size_t idx = 0; idx < scene_bodies(scene); idx++)
  {
    body_t *dot = scene_get_body(scene, idx);
    if (!body_is_movable(dot) && eat(idx, dot, pacman, scene))
    {
      idx--;
      return;
    }
  }
}

void move_pacman(double angle, double scale, body_t *pacman)
{
  vector_t i = (vector_t){PACMAN_STEP * scale, 0};
  vector_t move_vector = vec_rotate(i, angle);
  vector_t pacman_position = body_get_centroid(pacman);
  vector_t next_position = vec_add(pacman_position, move_vector);
  vector_t new_position = wrap_position(next_position);
  body_set_centroid(pacman, new_position);
  body_set_rotation(pacman, angle);
  body_set_velocity(pacman, vec_multiply(PACMAN_VELOCITY_SCALE, move_vector));
}

void handle(char key, key_event_type_t type, double held_time, body_t *pacman)
{
  double boost = VELOCITY_BOOST + held_time;

  if (type == KEY_PRESSED)
  {
    if (key == LEFT_ARROW)
    {
      move_pacman(M_PI, boost, pacman);
    }
    else if (key == RIGHT_ARROW)
    {
      move_pacman(0, boost, pacman);
    }
    else if (key == DOWN_ARROW)
    {
      move_pacman(M_PI * 3 / 2, boost, pacman);
    }
    else if (key == UP_ARROW)
    {
      move_pacman(M_PI * 1 / 2, boost, pacman);
    }
  }
}

void make_dot(scene_t *scene)
{
  list_t *circle = sprite_make_circle(CIRCLE_PRECISION);
  body_t *dot = body_init(circle, 0, DOT_COLOR);
  vector_t position = {.x = rand() % SCREEN_SIZE_X, .y = rand() % SCREEN_SIZE_Y};
  body_set_centroid(dot, position);
  body_set_movable(dot, false);
  body_set_camera_mode(dot, SCENE);
  scene_add_body(scene, dot);
}

int main(int argc, char *argv[])
{
  scene_t *scene = scene_init();
  for (int i = 0; i < INITIAL_DOTS; i++)
  {
    make_dot(scene);
  }
  list_t *pacman_shape = sprite_make_pacman(PACMAN_PRECISION);
  body_t *pacman = body_init(pacman_shape, 0, PACMAN_COLOR);
  vector_t position = (vector_t){.x = SCREEN_SIZE_X / 2, .y = SCREEN_SIZE_Y / 2};
  body_set_centroid(pacman, position);
  body_set_movable(pacman, true);
  body_set_camera_mode(pacman, FOLLOW);
  scene_add_body(scene, pacman);
  sdl_init(min, max);
  scene_add_camera_management(scene,
                              (camera_offset_func_t)camera_offset_func,
                              (camera_mover_func_t)camera_mover_func,
                              NULL);
  double dt;
  double time_until_add = DOT_ADD_PERIOD;
  sdl_event_args((void *)pacman);
  while (!sdl_is_done())
  {
    dt = time_since_last_tick();
    time_until_add -= dt;
    if (time_until_add <= 0)
    {
      make_dot(scene);
      time_until_add = DOT_ADD_PERIOD;
    }
    scene_tick(scene, dt);
    vector_t pacman_position = body_get_centroid(pacman);
    body_set_centroid(pacman, wrap_position(pacman_position));
    check_eat(pacman, scene);
    sdl_clear();
    sdl_on_key((key_handler_t)handle);
    sdl_render_scene(scene);
    sdl_show();
  }
  scene_free(scene);
}
