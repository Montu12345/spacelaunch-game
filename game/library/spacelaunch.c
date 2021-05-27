// #include "camerademo.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <SDL2/SDL_ttf.h>

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "collision.h"
#include "spacelaunch.h"
#include "game_build.h"
#include "game_actions.h"

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};
const rgb_color_t WAIT_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};
const int STARTING_KEY_VALUE = 0;
const int SHOOTING_STAR_TIME = 170;

void restart_game_2(double dt, scene_t *scene)
{
  scene_tick(scene, dt);
  game_actions_clear_scene(scene);
  sdl_clear();
  sdl_render_scene(scene);
  sdl_show();
}

int continue_game(scene_t *scene, game_state_t *aux)
{
  sdl_clear();
  sdl_event_args(aux);
  sdl_on_key((key_handler_t)handle_key_press);
  if (aux->game_state_num == A_KEY_VALUE)
  {
    return A_KEY_VALUE;
  }
  if (aux->game_state_num == Q_KEY_VALUE)
  {
    return Q_KEY_VALUE;
  }
  sdl_render_scene(scene);
  sdl_show();
  return 0;
}

int main(int argc, char *argv[])
{
  double dt;
  int t = 0;
  scene_t *scene = scene_init();
  game_build_draw_stary_night(scene);
  body_t *pacman = game_build_rocket(scene);
  game_build_draw_asteroids(scene, pacman);
  game_state_t *aux = game_state_init(pacman, STARTING_KEY_VALUE);
  sdl_init(min, max);

  while (!sdl_is_done())
  {
    t += 1;
    sdl_event_args(aux);
    dt = time_since_last_tick();
    if (t % SHOOTING_STAR_TIME == 0)
    {
      game_build_shooting_star(scene);
      t = 0;
    }
    if (game_actions_restart_game(pacman))
    {
      list_t *background_list = sprite_make_rect(0,
                                                 SCREEN_SIZE_X,
                                                 0,
                                                 SCREEN_SIZE_Y);
      body_t *background = body_init(background_list,
                                     0,
                                     WAIT_BACKGROUND_COLOR);
      game_actions_clear_scene(scene);
      scene_add_body(scene, background);
      while (aux->game_state_num == 0 && !sdl_is_done())
      {
        create_words();
        aux->game_state_num = continue_game(scene, aux);
      }
      if (aux->game_state_num == 2)
      {
        break;
      }
      restart_game_2(dt, scene);
      game_build_draw_stary_night(scene);
      pacman = game_build_rocket(scene);
      game_build_draw_asteroids(scene, pacman);
      aux = game_actions_game_restart_aux(aux, pacman);
      sdl_event_args(aux);
      scene_tick(scene, dt);
    }
    scene_tick(scene, dt);
    sdl_clear();
    sdl_on_key((key_handler_t)handle_key_press);
    sdl_render_scene(scene);
    sdl_show();
  }

  scene_free(scene);
}