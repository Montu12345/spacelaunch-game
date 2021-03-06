#ifndef __GAME_ACTIONS_H__
#define __GAME_ACTIONS_H__
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "game_build.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "text.h"

extern const int INITIAL_TIME;

typedef enum {
  SCREEN_GAME_OVER,
  SCREEN_GAME_WIN,
  SCREEN_GAME,
  SCREEN_QUIT,
  SCREEN_START,
  SCREEN_HELP,
  SCREEN_CONTINUE,
} screen_t;

typedef struct game_texts {
  text_t *score;
  text_t *timer;
  text_t *health;
  text_t *level;
  text_t *power_up;
} game_texts_t;
 
/**
 * Properties to pass into the handler
 *  focal_body -- the focal_body body
 */
typedef struct game_state {
  scene_t *scene;
  body_t *rocket;
  body_t *score_display;
  game_texts_t *texts;

  int ticks;
  double timer;

  bool needs_restart;
  screen_t current_screen;

  int score;
  int health;
  int level;
  vector_t arena_max;

  int thrust_ticks_remaining;

  bool quit_game;
} game_state_t;

/**
 * Sets the beginning of the game scene.
 *
 * @param state the game state
 * @param screen_min the minimum screen size
 * @param screen_max the maximum screen size
 */
void game_setup(game_state_t *state, vector_t screen_min, vector_t screen_max);

/**
 * Handles the different key presses
 *
 * @param key the key that was pressed
 * @param type type of key press
 * @param held_time time that the key is held down
 * @param aux game state
 */
void handle_key_press(char key, key_event_type_t type, double held_time,
                      game_state_t *aux);

/**
 * Creates a game state
 *
 * @param focal_body the body that the screen follows
 * @param game_state boolean 
 * 
 * @return game state
 */
game_state_t *game_state_init(body_t *focal_body, bool game_state);

/**
 * Offsets everything in the screen.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the info passed to body_init()
 */
vector_t game_actions_camera_offset_func(body_t *focal_body, void *aux);

/**
 * Moves everything in the scene except the focal_body
 *
 * @param body a pointer to a body returned from body_init()
 * @return the info passed to body_init()
 */
vector_t game_actions_camera_mover_func(vector_t offset, body_t *body);

/**
 * Moves the focal_body.
 *
 * @param angle angle shift of the focal_body
 * @param scale the scale to move the focal_body
 * @param focal_body focal_body in the scene
 */
void game_actions_thrust_rocket(double angle, double scale,
                                game_state_t *state);

/**
 * Creates a collision between the rockey and an asteroid
 *
 * @param rockey focal_body on the screen
 * @param asteroid asteroid on the screen
 * @param axis axis to rotate
 * @param state game state
 */
void game_actions_physics_collision(body_t *focal_body, body_t *asteroid,
                                    vector_t axis, game_state_t *state);

/**
 * Adds the collision force to the scene, focal_body, and asteroid
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param focal_body focal_body on the screen
 * @param asteroid asteroid on the screen
 * @param state game state
 */
void game_actions_rocket_obstacles_collision(scene_t *scene, body_t *focal_body,
                                             body_t *asteroid,
                                             game_state_t *state);

/**
 * Detecs collision with the borders of the game
 *
 * @param state the game state
 * @param fence borders on screen
 */
void game_actions_rocket_fence_collision(game_state_t *state, body_t *fence);

/**
 * Detects collision with the endzone of the game
 *
 * @param state the game state
 * @param endzone endzone of the game
 */
void game_actions_rocket_endzone_collision(game_state_t *state, body_t *endzone);

/**
 * Changes the game state if necessary to GAME_OVER
 * if the game has ended.
 * Based on the position of the focal_body.
 *
 * @param state the game state
 */
void game_actions_check_for_game_over(game_state_t *state);

/**
 * Creates the new health bar
 *
 * @param state the game state
 * @param scale new size of the health bar
 */
void game_actions_new_health(game_state_t *state, int scale);

/**
 * Sets up the welcome page
 *
 * @param state the game state
 */
void game_beginning_setup(game_state_t *state);

/**
 * Sets up the help page
 *
 * @param state the game state
 */
void game_help_setup(game_state_t *state);

/**
 * Removes the help screen from the scene.
 *
 * @param state the game state
 */
void game_actions_help_end(game_state_t *state);

/**
 * Creates win screen.
 *
 * @param state the game state
 */
void game_actions_game_win(game_state_t *state);

/**
 * Removes the text that states a powerup has been created.
 *
 * @param state the game state
 */
void game_actions_remove_power_up_text(game_state_t *state, int index);

#endif // #ifndef __GAME_ACTIONS_H__