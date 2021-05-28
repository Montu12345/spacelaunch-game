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
#include "camerademo.h"

const int SCREEN_SIZE_X = 1000;
const int SCREEN_SIZE_Y = 500;
const vector_t min = {.x = 0, .y = 0};
const vector_t max = {.x = SCREEN_SIZE_X, .y = SCREEN_SIZE_Y};
const rgb_color_t WAIT_BACKGROUND_COLOR = {.r = 0, .g = 0, .b = 0};
const int STARTING_KEY_VALUE = 0;
const int SHOOTING_STAR_TIME = 170;

const int TEXT_WIDTH = 100;
const int TEXT_HEIGHT = 30;

const vector_t SCORE_POSITION = {.x = SCREEN_SIZE_X / 2.0 - TEXT_WIDTH / 2.0, .y = 5};
const vector_t SCORE_DIMENTIONS = {.x = TEXT_WIDTH, .y = TEXT_HEIGHT};

vector_t TIMER_POSITION = {.x = SCREEN_SIZE_X - TEXT_WIDTH - 15, .y = 5};
vector_t TIMER_DIMENTIONS = {.x = TEXT_WIDTH, .y = TEXT_HEIGHT};

const vector_t FINAL_SCORE_POS = {.x = SCREEN_SIZE_X / 2.0 - TEXT_WIDTH / 2.0, .y = SCREEN_SIZE_Y / 2.0 - TEXT_HEIGHT / 2.0};
const vector_t FINAL_SCORE_DIM = {.x = TEXT_WIDTH, .y = TEXT_HEIGHT};

void display_score(game_state_t *state)
{
    sdl_create_words(SCORE_POSITION, SCORE_DIMENTIONS, "Score: ", state->score);
}

void display_timer(game_state_t *state){
    sdl_create_words(TIMER_POSITION, TIMER_DIMENTIONS, "Timer: ", (int)state->timer);
}

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

  state->timer += 0.1;
  game_actions_check_for_game_over(state);
  display_score(state);
  display_timer(state);
  sdl_render_scene(state->scene);
  sdl_clear();
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
    sdl_create_words(FINAL_SCORE_POS, FINAL_SCORE_DIM, "Score: ", state->score);
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
