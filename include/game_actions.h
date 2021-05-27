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

/**
 * Properties to pass into the handler
 *  rocket -- the rocket body
 *  gamae_state -- int value to determine if the game should continue or end
*/
typedef struct space_aux space_aux_t;

/**
 * NEED TO DO!!!!!!!!!!
 *
 * @param body a pointer to a body returned from body_init()
 * @return the info passed to body_init()
 */
vector_t game_actions_camera_offset_func_2(body_t *focal_body, void *aux);

/**
 * NEED TO DO!!!!!!!!!!
 *
 * @param body a pointer to a body returned from body_init()
 * @return the info passed to body_init()
 */
vector_t game_actions_camera_mover_func_2(vector_t offset, body_t *body);

/**
 * Moves the rocket.
 *
 * @param angle angle shift of the rocket
 * @param scale the scale to move the rocket
 * @param rocket rocket in the scene
 */
void game_actions_move_rocket(double angle, double scale, body_t *rocket);

/**
 * Creates a collision between the rockey and an asteroid
 *
 * @param rockey rocket on the screen
 * @param asteroid asteroid on the screen
 * @param axis NOT SURE!!!!!!!!!!
 */
void game_actions_physics_collision(body_t *rocket, body_t *asteroid, vector_t axis);

/**
 * Adds the collision force to the scene, rocket, and asteroid
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param rocket rocket on the screen
 * @param asteroid asteroid on the screen
 */
void game_actions_rocket_obstacles_collision(scene_t *scene, body_t *rocket, body_t *asteroid);

/**
 * Whether the game should be continued or not.
 * Based on the position of the rocket.
 *
 * @param body a pointer to a body returned from body_init()
 * @return true if game should continue; false if game should end
 */
bool game_actions_restart_game(body_t *rocket);

/**
 * Clears all objects from the screen
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void game_actions_clear_scene(scene_t *scene);

/**
 * Updates the aux if the game continues
 * 
 * @param aux existing aux
 * @param rocket rocket on the screen
 * @return space_aux_t 
 */
space_aux_t *game_actions_game_restart_aux(space_aux_t *aux, body_t *rocket);

#endif // #ifndef __GAME_ACTIONS_H__