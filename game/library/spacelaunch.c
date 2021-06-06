#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "body.h"
#include "camerademo.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "game_actions.h"
#include "game_build.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "spacelaunch.h"
#include "sprite.h"
#include "text.h"

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};
const rgb_color_t WAIT_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};
const int SHOOTING_STAR_ADD_INTERVAL = 170;

const vector_t END_GAME_SCORE_DIMENSIONS = {.x = 300, .y = 100};
const int END_GAME_SCORE_SIZE = 100;
const vector_t END_GAME_SCORE_POSITION = {.x = SCREEN_SIZE_X / 2.0 - 300 / 2.0,
                                          .y = SCREEN_SIZE_Y / 2.0 - 40};
const vector_t END_GAME_CONT_DIMENSIONS = {.x = 500, .y = 100};
const int END_GAME_CONT_SIZE = 100;
const vector_t END_GAME_CONT_POSITION = {.x = SCREEN_SIZE_X / 2.0 - 500 / 2.0,
                                         .y = SCREEN_SIZE_Y / 2.0 - 100 / 2.0 +
                                              50};

const vector_t WIN_GAME_CONT_DIMENSIONS = {.x = 700, .y = 100};
const vector_t WIN_GAME_CONT_POSITION = {.x = SCREEN_SIZE_X / 2.0 - 700 / 2.0,
                                         .y = SCREEN_SIZE_Y / 2.0 - 100 / 2.0 +
                                              50};

const int TEXT_WIDTH = 100;
const int TEXT_HEIGHT = 30;

void update_score(game_state_t *state)
{
  if (!state->needs_restart)
  {
    int new_score = state->health / 10 - 10;
    if (new_score < 0)
    {
      new_score = 0;
    }
    state->score = new_score;
  }
}

void screen_game_render(game_state_t *state)
{
  if (state->needs_restart)
  {
    game_setup(state, min, max);
  }
  else
  {
    game_actions_help_end(state);
    game_update_texts(state);
  }
  state->ticks += 1;
  if (state->thrust_ticks_remaining > 0)
  {
    state->thrust_ticks_remaining -= 1;
  }

  if (state->ticks % SHOOTING_STAR_ADD_INTERVAL == 0)
  {
    game_build_shooting_star(state->scene);
  }

  state->timer += 0.1;
  game_actions_check_for_game_over(state);
}

void screen_game_over_render(game_state_t *state)
{
  int ticks = state->ticks;
  if (ticks == 0)
  {
    scene_free(state->scene);
    state->scene = scene_init();
    state->rocket = NULL;
    state->needs_restart = false;
    state->level = 1;
    text_t *score = text_init("SCORE: ", END_GAME_SCORE_POSITION,
                              END_GAME_SCORE_SIZE, state->score,
                              END_GAME_SCORE_DIMENSIONS);
    scene_add_text(state->scene, score);
    game_build_lost_background(state);
  }
  state->ticks += 1;
}

void screen_game_win_render(game_state_t *state)
{
  int ticks = state->ticks;
  if (ticks == 0)
  {
    scene_free(state->scene);
    state->scene = scene_init();
    game_build_won_background(state);
    state->rocket = NULL;
    state->needs_restart = false;
    text_t *score = text_init("SCORE: ", END_GAME_SCORE_POSITION,
                              END_GAME_SCORE_SIZE, state->score,
                              END_GAME_SCORE_DIMENSIONS);
    scene_add_text(state->scene, score);
  }
  state->ticks += 1;
}

void game_state_free(game_state_t *state)
{
  scene_free(state->scene);
  free(state->texts);
  free(state);
}

int main(int argc, char *argv[])
{
  // Initialize random number generator
  srand(time(NULL));

  // Initialize the game state
  game_state_t *state = malloc(sizeof(game_state_t));
  state->texts = NULL;
  state->current_screen = SCREEN_START;
  state->needs_restart = true;
  state->thrust_ticks_remaining = 0;
  state->quit_game = false;

  // Initialize SDL
  TTF_Init();
  sdl_init(min, max);
  sdl_event_args(state);
  sdl_on_key((key_handler_t)handle_key_press);

  // Start the music
  SDL_Init(SDL_INIT_AUDIO);
  SDL_AudioSpec wavSpec;
  Uint32 wavLength;
  Uint8 *wavBuffer;
  SDL_LoadWAV("Interlude.wav", &wavSpec, &wavBuffer, &wavLength);

  SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

  SDL_QueueAudio(deviceId, wavBuffer, wavLength);
  SDL_PauseAudioDevice(deviceId, 0);

  // Render the correct screen each tick.
  while (!sdl_is_done())
  {
    if (state->quit_game)
    {
      break;
    }
    double dt = time_since_last_tick();
    update_score(state);
    if (state->needs_restart)
    {
      state->ticks = 0;
    }
    switch (state->current_screen)
    {
    case SCREEN_START:
      game_beginning_setup(state);
      break;
    case SCREEN_HELP:
      game_help_setup(state);
      break;
    case SCREEN_GAME:
      screen_game_render(state);
      scene_tick(state->scene, dt);
      break;
    case SCREEN_GAME_OVER:
      screen_game_over_render(state);
      break;
    case SCREEN_GAME_WIN:
      screen_game_win_render(state);
      break;

    default:
      break;
    }
    sdl_render_scene(state->scene);
  }
  TTF_Quit();
  game_state_free(state);

  SDL_CloseAudioDevice(deviceId);
  // SDL_Quit();
}
