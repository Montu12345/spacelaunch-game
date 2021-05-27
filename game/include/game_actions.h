#ifndef __GAME_ACTIONS_H__
#define __GAME_ACTIONS_H__
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <unistd.h>

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "collision.h"
#include "game_build.h"

const int GA_SCREEN_SIZE_X;
const int GA_SCREEN_SIZE_Y;
const vector_t GA_min;
const vector_t GA_max;
const int GA_STARTING_KEY_VALUE;
const double GA_ROCKET_ELASTICITY;
const int GA_ROCKET_STEP;
const double GA_ROCKET_VELOCITY_SCALE;
const rgb_color_t GA_RED;
const rgb_color_t GA_YELLOW;
const int GA_ROCKET_RADIUS;
const int GA_MAX_OBSTACLES_SCREEN_SIZE_X;
const int GA_MAX_OBSTACLES_SCREEN_SIZE_Y;
const int GA_MIN_OBSTACLES_SCREEN_SIZE_Y;
const int A_KEY_VALUE;
const int Q_KEY_VALUE;

typedef enum
{
  SCREEN_GAME_OVER,
  SCREEN_GAME,
  SCREEN_QUIT,
} screen_t;

/**
 * Properties to pass into the handler
 *  focal_body -- the focal_body body
*/
typedef struct game_state
{
  body_t *focal_body;
  int ticks;
  scene_t *scene;
  body_t *rocket;
  bool needs_restart;
  screen_t current_screen;
} game_state_t;

void game_setup(game_state_t *state);

void handle_key_press(char key, key_event_type_t type, double held_time, game_state_t *aux);

game_state_t *game_state_init(body_t *focal_body, bool game_state);

/**
 * NEED TO DO!!!!!!!!!!
 *
 * @param body a pointer to a body returned from body_init()
 * @return the info passed to body_init()
 */
vector_t game_actions_camera_offset_func(body_t *focal_body, void *aux);

/**
 * NEED TO DO!!!!!!!!!!
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
void game_actions_thrust_rocket(double angle, double scale, body_t *focal_body);

/**
 * Creates a collision between the rockey and an asteroid
 *
 * @param rockey focal_body on the screen
 * @param asteroid asteroid on the screen
 * @param axis NOT SURE!!!!!!!!!!
 */
void game_actions_physics_collision(body_t *focal_body, body_t *asteroid, vector_t axis);

/**
 * Adds the collision force to the scene, focal_body, and asteroid
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param focal_body focal_body on the screen
 * @param asteroid asteroid on the screen
 */
void game_actions_rocket_obstacles_collision(scene_t *scene, body_t *focal_body, body_t *asteroid);

/**
 * Changes the game state if necessary to GAME_OVER
 * if the game has ended. 
 * Based on the position of the focal_body.
 *
 * @param state the game state
 */
void game_actions_check_for_game_over(game_state_t *state);

/**
 * Updates the aux if the game continues
 * 
 * @param aux existing aux
 * @param focal_body focal_body on the screen
 * @return game_state_t 
 */
game_state_t *game_actions_game_restart_aux(game_state_t *aux, body_t *focal_body);

#endif // #ifndef __GAME_ACTIONS_H__