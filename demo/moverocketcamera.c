#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include "moverocket.h"
#include "collision.h"
#include "forces.h"

// Launch rocket demo

const vector_t ACCELERATION = {.x = 0, .y = -5000};

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;

const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};

const vector_t INITIAL_POS = {.x = SCREEN_SIZE_X / 10, .y = SCREEN_SIZE_Y / 10};

const int PACMAN_STEP = 10;
const int CIRCLE_PRECISION = 10;
const int PACMAN_RADIUS = 30;
const double PACMAN_VELOCITY_SCALE = 30;
const int INITIAL_DOTS = 30;
const double EAT_TOLERANCE = 10.0;
const double DOT_ADD_PERIOD = 1.0;
const double VELOCITY_BOOST = 1.0;

const double PACMAN_MASS = 2.0;
const double DOT_MASS = 1.0;

const rgb_color_t PACMAN_COLOR = {.r = 0, .g = 0, .b = 0};
const rgb_color_t DOT_COLOR = {.r = 0, .g = 0, .b = 0};

vector_t camera_offset_func(body_t *focal_body, void *aux)
{
  vector_t center = vec_multiply(0.5, max);
  vector_t offset = vec_subtract(center, body_get_centroid(focal_body));

  // printf("center: <%f %f>\n", center.x, center.y);
  return offset;
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

void move_rocket(double angle, double scale, body_t *pacman)
{
  vector_t i = (vector_t){PACMAN_STEP * scale, 0};
  vector_t move_vector = vec_rotate(i, angle);
  // vector_t pacman_position = body_get_centroid(pacman);
  // vector_t next_position = vec_add(pacman_position, move_vector);
  // body_set_centroid(pacman, next_position);
  body_set_rotation(pacman, angle);

  body_add_impulse(pacman, vec_multiply(PACMAN_VELOCITY_SCALE, move_vector));
  // body_set_velocity(pacman, vec_multiply(PACMAN_VELOCITY_SCALE, move_vector));
}

void handle(char key, key_event_type_t type, double held_time, body_t *pacman)
{
  double boost = VELOCITY_BOOST + held_time;

  if (type == KEY_PRESSED)
  {
    if (key == SPACEBAR)
    {
      move_rocket(M_PI * 1 / 4, 1, pacman);
    }
    else if (key == LEFT_ARROW)
    {
      move_rocket(M_PI, boost, pacman);
    }
    else if (key == RIGHT_ARROW)
    {
      move_rocket(0, boost, pacman);
    }
    else if (key == DOWN_ARROW)
    {
      move_rocket(M_PI * 3 / 2, boost, pacman);
    }
    else if (key == UP_ARROW)
    {
      move_rocket(M_PI * 1 / 2, boost, pacman);
    }
  }
}

void half_destructive_handler(body_t *keep, body_t *erase, vector_t axis)
{
  body_remove(erase);
}

void create_half_desructive_collision(scene_t *scene, body_t *keep, body_t *erase)
{
  create_collision(scene, keep, erase, (collision_handler_t)half_destructive_handler, NULL, NULL);
}

void make_dot(scene_t *scene, body_t *pacman)
{
  list_t *circle = sprite_make_circle(CIRCLE_PRECISION);
  body_t *dot = body_init(circle, DOT_MASS, DOT_COLOR);
  vector_t position = {.x = rand() % SCREEN_SIZE_X, .y = rand() % SCREEN_SIZE_Y};
  body_set_centroid(dot, position);
  body_set_movable(dot, false);
  body_set_camera_mode(dot, SCENE);
  scene_add_body(scene, dot);
}

void add_dots(scene_t *scene, body_t *pacman)
{
  for (int i = 0; i < INITIAL_DOTS; i++)
  {
    make_dot(scene, pacman);
  }
}

body_t *add_pacman(scene_t *scene)
{
  list_t *pacman_shape = sprite_make_circle(PACMAN_RADIUS);
  body_t *pacman = body_init(pacman_shape, PACMAN_MASS, PACMAN_COLOR);

  body_set_centroid(pacman, INITIAL_POS);
  body_set_movable(pacman, true);
  body_set_camera_mode(pacman, FOLLOW);
  body_set_static_texture_path(pacman, "game/textures/rocket/rocket1.png");
  scene_add_body(scene, pacman);

  return pacman;
}

int main(int argc, char *argv[])
{
  // Initialize random number generator
  srand(time(NULL));

  // Initialize scene
  sdl_init(min, max);
  scene_t *scene = scene_init();

  // Add elements to the scene
  body_t *pacman = add_pacman(scene);
  add_dots(scene, pacman);

  // Add camera management
  scene_add_camera_management(scene,
                              (camera_offset_func_t)camera_offset_func,
                              (camera_mover_func_t)camera_mover_func,
                              NULL, NULL);
  scene_set_focal_body(scene, pacman);

  double dt;
  double time_until_add = DOT_ADD_PERIOD;
  sdl_event_args((void *)pacman);

  while (!sdl_is_done())
  {
    dt = time_since_last_tick();

    // Add dot every DOT_ADD_PERIOD seconds.
    time_until_add -= dt;
    if (time_until_add <= 0)
    {
      make_dot(scene, pacman);
      time_until_add = DOT_ADD_PERIOD;
    }

    scene_tick(scene, dt);
    sdl_on_key((key_handler_t)handle);
    sdl_render_scene(scene);
  }

  scene_free(scene);
}