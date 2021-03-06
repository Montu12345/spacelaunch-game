#include "moverocket.h"
#include "collision.h"
#include "forces.h"
#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

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

vector_t camera_offset_func(body_t *focal_body, void *aux) {
  vector_t center = vec_multiply(0.5, max);
  return vec_subtract(center, body_get_centroid(focal_body));
}

vector_t camera_mover_func(vector_t offset, body_t *body) {
  camera_mode_t camera_mode = body_get_camera_mode(body);

  switch (camera_mode) {
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

void move_rocket(double angle, double scale, body_t *pacman) {
  vector_t i = (vector_t){PACMAN_STEP * scale, 0};
  vector_t move_vector = vec_rotate(i, angle);
  body_add_impulse(pacman, vec_multiply(PACMAN_VELOCITY_SCALE, move_vector));
}

void handle(char key, key_event_type_t type, double held_time, body_t *pacman) {
  // double boost = VELOCITY_BOOST + held_time;
  double boost = 1;

  if (type == KEY_PRESSED) {
    if (key == SPACEBAR) {
      move_rocket(M_PI * 1 / 4, 1, pacman);
    } else if (key == LEFT_ARROW) {
      move_rocket(M_PI, boost, pacman);
    } else if (key == RIGHT_ARROW) {
      move_rocket(0, boost, pacman);
    } else if (key == DOWN_ARROW) {
      move_rocket(M_PI * 3 / 2, boost, pacman);
    } else if (key == UP_ARROW) {
      move_rocket(M_PI * 1 / 2, boost, pacman);
    }
  }
}

void half_destructive_handler(body_t *keep, body_t *erase, vector_t axis) {
  body_remove(erase);
}

void create_half_desructive_collision(scene_t *scene, body_t *keep,
                                      body_t *erase) {
  create_collision(scene, keep, erase,
                   (collision_handler_t)half_destructive_handler, NULL, NULL);
}

void make_dot(scene_t *scene, body_t *pacman) {
  list_t *circle = sprite_make_circle(CIRCLE_PRECISION);
  body_t *dot = body_init(circle, DOT_MASS, DOT_COLOR);
  vector_t position = {.x = rand() % SCREEN_SIZE_X,
                       .y = rand() % SCREEN_SIZE_Y};
  body_set_centroid(dot, position);
  body_set_movable(dot, false);
  scene_add_body(scene, dot);
}

int main(int argc, char *argv[]) {
  scene_t *scene = scene_init();
  list_t *pacman_shape = sprite_make_pacman(PACMAN_RADIUS);
  body_t *pacman = body_init(pacman_shape, PACMAN_MASS, PACMAN_COLOR);
  for (int i = 0; i < INITIAL_DOTS; i++) {
    make_dot(scene, pacman);
  }
  vector_t position = INITIAL_POS;
  body_set_centroid(pacman, position);
  body_set_movable(pacman, true);
  scene_add_body(scene, pacman);
  sdl_init(min, max);
  TTF_Init();
  // scene_add_camera_management(scene,
  //                             (camera_offset_func_t)camera_offset_func,
  //                             (camera_mover_func_t)camera_mover_func,
  //                             NULL, NULL);
  double dt;
  double time_until_add = DOT_ADD_PERIOD;
  SDL_Window *window =
      SDL_CreateWindow("SDL_ttf in SDL2", SDL_WINDOWPOS_UNDEFINED,
                       SDL_WINDOWPOS_UNDEFINED, 640, 480, 0);
  SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);
  TTF_Font *font = TTF_OpenFont("Roboto-Black.ttf", 25);
  SDL_Color color = {255, 255, 255};
  SDL_Surface *surface = TTF_RenderText_Solid(font, "HELLO", color);
  // SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer, surface);
  sdl_event_args((void *)pacman);
  while (!sdl_is_done()) {
    dt = time_since_last_tick();
    time_until_add -= dt;
    if (time_until_add <= 0) {
      make_dot(scene, pacman);
      time_until_add = DOT_ADD_PERIOD;
    }
    scene_tick(scene, dt);
    sdl_clear();
    sdl_on_key((key_handler_t)handle);
    sdl_render_scene(scene);
    sdl_show();
  }
  scene_free(scene);
  SDL_FreeSurface(surface);
  // SDL_DestroyTexture(texture);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  TTF_CloseFont(font);
  SDL_Quit();
}