#ifndef __GAME_BUILD_H__
#define __GAME_BUILD_H__

#include <SDL2/SDL_ttf.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "game_actions.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"

typedef struct game_state game_state_t;

enum space_body_type_t *space_body_type_init(enum space_body_type_t b);

/**
 * Makes a shooting star.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void game_build_shooting_star(scene_t *scene);

/**
 * Draws the background and the stars.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void game_build_draw_starry_night(scene_t *scene);

/**
 * Draws the asteroids.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_draw_asteroids(game_state_t *state);

/**
 * Creates the rocket.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param state the game state
 * @param type void* pointing to a space_body_type_t for the type of rocket.
 * @return pointer to the rocket
 */
body_t *game_build_rocket(scene_t *scene, game_state_t *state, void *type);

/**
 * Creates the sky.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void game_build_sky(scene_t *scene);

/**
 * Creates the stars that are in the sky.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void game_build_stars(scene_t *scene);

/**
 * Generates the black barrier around the game frame.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_fence(game_state_t *state);

/**
 * Generates the zone at the far right of the frame
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_endzone(game_state_t *state);

/**
 * Makes a single asteroid with the given centroid and adds it to the screen.
 * The asteroid is randomly assigned to be a GOOD_OBSTACLE or BAD_OBSTACLE.
 *
 * @param state a pointer to a game_state_t struct
 * @param centroid a pointer to the rocket in the scene.
 */
void game_build_asteroid(game_state_t *state, vector_t centroid);

/**
 * Builds the score, timer, health, and level messages
 * and adds them to the state's screen.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_display_text(game_state_t *state);

/**
 * Updates the value of the game texts from game_build_display_text
 * according to the current state.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_update_texts(game_state_t *state);

/**
 * Adds the health bar to the top left of the screen.
 * The bar's width depends on the health of the rocket.
 *
 * @param state a pointer to a game_state_t struct
 */
body_t *game_build_score_keeper(scene_t *scene, double height, double width);

/**
 * Adds the help menu to the state's scene.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_help(game_state_t *state);

/**
 * Adds a blue background rectangle to the state's scene.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_blue_back(game_state_t *state);

/**
 * Adds the help menu to the scene.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_help_screen(game_state_t *state);

/**
 * Adds two rockets to the state's scene, oriented
 * upward with animated flames. To be built in conjunction
 * with the information screens. The rockets do not move.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_stationary_rockets(game_state_t *state);

/**
 * Adds the initial "welcome to spacelaunch" backdrop
 * to the state's scene.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_welcome_background(game_state_t *state);

/**
 * Adds the "You Won!" backdrop
 * to the state's scene.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_won_background(game_state_t *state);

/**
 * Adds the "You Lost!" backdrop
 * to the state's scene.
 *
 * @param state a pointer to a game_state_t struct
 */
void game_build_lost_background(game_state_t *state);

/**
 * Adds a message to the screen which will disappear shortly.
 *
 * @param state a pointer to a game_state_t struct
 * @param words the contents of the message to display
 */
void game_build_powerup_text(game_state_t *state, char *words);

#endif // #ifndef __GAME_BUILD_H__