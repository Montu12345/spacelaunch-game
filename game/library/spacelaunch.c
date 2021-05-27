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

void screen_game_render(game_state_t *state)
{
  if (state->needs_restart)
  {
    game_setup(state);
  }
  state->ticks += 1;
  if (state->ticks % SHOOTING_STAR_TIME == 0)
  {
    game_build_shooting_star(state->scene);
  }
  game_actions_check_for_game_over(state);
}

void screen_game_over_render(game_state_t *state)
{
  if (state->ticks == 0)
  {
    scene_free(state->scene);
    state->scene = scene_init();
    state->rocket = NULL;
    state->needs_restart = false;

    list_t *screen_rect = sprite_make_rect(0, SCREEN_SIZE_X, 0, SCREEN_SIZE_Y);
    body_t *background = body_init(screen_rect, 0, WAIT_BACKGROUND_COLOR);
    scene_add_body(state->scene, background);

    // TODO: Why does this make text rendering work? Reducing redundant calls breaks functionality.
    sdl_render_scene(state->scene);
    sdl_clear();
    sdl_render_scene(state->scene);
    sdl_clear();
    sdl_render_scene(state->scene);
    create_words();
  }
  state->ticks += 1;
}

int main(int argc, char *argv[])
{
  // Initialize the game state
  game_state_t *state = malloc(sizeof(game_state_t));
  state->current_screen = SCREEN_GAME;
  state->needs_restart = true;

  // Initialize SDL
  sdl_init(min, max);
  sdl_event_args(state);
  sdl_on_key((key_handler_t)handle_key_press);

  // Render the correct screen each tick.
  while (!sdl_is_done())
  {
    double dt = time_since_last_tick();
    if (state->needs_restart)
    {
      state->ticks = 0;
    }
    switch (state->current_screen)
    {
    case SCREEN_GAME:
      screen_game_render(state);
      scene_tick(state->scene, dt);
      sdl_render_scene(state->scene);
      break;
    case SCREEN_GAME_OVER:
      screen_game_over_render(state);
      break;
    default:
      break;
    }
  }

  scene_free(state->scene);
  free(state);
}
