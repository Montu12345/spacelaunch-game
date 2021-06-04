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

const vector_t END_GAME_SCORE_DIMENTIONS = {.x = 300, .y = 100};
const int END_GAME_SCORE_SIZE = 100;
const vector_t END_GAME_SCORE_POSITION = {.x = SCREEN_SIZE_X / 2.0 - 300 / 2.0,
                                          .y = SCREEN_SIZE_Y / 2.0 - 100};
const vector_t END_GAME_CONT_DIMENTIONS = {.x = 500, .y = 100};
const int END_GAME_CONT_SIZE = 100;
const vector_t END_GAME_CONT_POSITION = {.x = SCREEN_SIZE_X / 2.0 - 500 / 2.0,
                                         .y = SCREEN_SIZE_Y / 2.0 - 100 / 2.0 +
                                              50};

const int TEXT_WIDTH = 100;
const int TEXT_HEIGHT = 30;

void update_score(game_state_t *state) {
  int new_score = state->health / 50 - 2;
  if (new_score < 0) {
    new_score = 0;
  }
  state->score = new_score;
}

void screen_game_render(game_state_t *state) {
  if (state->needs_restart) {
    game_setup(state, min, max);
  } else {
    game_actions_help_end(state);
    game_update_texts(state);
  }
  state->ticks += 1;
  if (state->thrust_ticks_remaining > 0) {
    state->thrust_ticks_remaining -= 1;
  }

  if (state->ticks % SHOOTING_STAR_ADD_INTERVAL == 0) {
    game_build_shooting_star(state->scene);
  }

  state->timer += 0.1;
  game_actions_check_for_game_over(state);
}

void screen_game_over_render(game_state_t *state) {
  int ticks = state->ticks;
  if (ticks == 0) {
    scene_free(state->scene);
    state->scene = scene_init();
    state->rocket = NULL;
    state->needs_restart = false;
    text_t *score = text_numbers_init("Score: ", END_GAME_SCORE_POSITION,
                                      END_GAME_SCORE_SIZE, state->score,
                                      END_GAME_SCORE_DIMENTIONS);
    scene_add_text(state->scene, score);
    text_t *continue_playing =
        text_words_init("To continue playing press 'A'", END_GAME_CONT_POSITION,
                        END_GAME_CONT_SIZE, END_GAME_CONT_DIMENTIONS);
    scene_add_text(state->scene, continue_playing);
    list_t *screen_rect = sprite_make_rect(min.x, max.x, min.y, max.y);
    body_t *background = body_init(screen_rect, 0, WAIT_BACKGROUND_COLOR);
    scene_add_body(state->scene, background);
  }
  state->ticks += 1;
}

void free_game_state(game_state_t *state) {
  scene_free(state->scene);
  free(state->texts);
  free(state);
}

int main(int argc, char *argv[]) {
  // Initialize random number generator
  srand(time(NULL));

  // Initialize the game state
  game_state_t *state = malloc(sizeof(game_state_t));
  state->current_screen = SCREEN_START;
  state->needs_restart = true;
  state->thrust_ticks_remaining = 0;

  // Initialize SDL
  TTF_Init();
  sdl_init(min, max);
  sdl_event_args(state);
  sdl_on_key((key_handler_t)handle_key_press);

  // Start the music
  // SDL_Init(SDL_INIT_AUDIO);
  // SDL_AudioSpec wavSpec;
  // Uint32 wavLength;
  // Uint8 *wavBuffer;
  // SDL_LoadWAV("Interlude.wav", &wavSpec, &wavBuffer, &wavLength);

  // SDL_AudioDeviceID deviceId = SDL_OpenAudioDevice(NULL, 0, &wavSpec, NULL, 0);

  // SDL_QueueAudio(deviceId, wavBuffer, wavLength);
  // SDL_PauseAudioDevice(deviceId, 0);

  // Render the correct screen each tick.
  while (!sdl_is_done()) {
    double dt = time_since_last_tick();
    update_score(state);
    if (state->needs_restart) {
      state->ticks = 0;
    }
    switch (state->current_screen) {
    case SCREEN_START:
      game_beginning_setup(state);
      sdl_render_scene(state->scene);
      break;
    case SCREEN_HELP:
      game_help_setup(state);
      sdl_render_scene(state->scene);
      break;
    case SCREEN_GAME:
      screen_game_render(state);
      scene_tick(state->scene, dt);
      sdl_render_scene(state->scene);
      break;
    case SCREEN_GAME_OVER:
      screen_game_over_render(state);
      sdl_render_scene(state->scene);
      break;

    default:
      break;
    }
  }
  TTF_Quit();
  free_game_state(state);
}
