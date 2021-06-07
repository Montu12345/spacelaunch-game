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
 * Draws the asteroids.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param rocket a pointer to the rocket in the scene
 */
void game_build_draw_asteroids(game_state_t *state);

/**
 * Draws the background and the stars.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void game_build_draw_stary_night(scene_t *scene);

/**
 * Creates the rocket.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param state the game state
 * @return pointer to the rocket
 */
body_t *game_build_rocket(scene_t *scene, game_state_t *state, void* type);

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

void game_build_fence(game_state_t *state);

void game_build_endzone(game_state_t *state);

/**
 * Makes the asteroids.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param rocket a pointer to the rocket in the scene.
 */
void game_build_asteroid(game_state_t *state, vector_t centroid);

void game_build_display_text(game_state_t *state);

void game_update_texts(game_state_t *state);

body_t *game_build_score_keeper(scene_t *scene, double height, double width);

void game_build_help(game_state_t *state);

void game_build_blue_back(game_state_t *state);

void game_build_help_screen(game_state_t *state);

void game_build_stationary_rockets(game_state_t *state);

void game_build_welcome_background(game_state_t *state);

void game_build_won_background(game_state_t *state);

void game_build_lost_background(game_state_t *state);

void game_build_powerup_text(game_state_t *state, char *words);

void game_build_display_level(game_state_t *state);

#endif // #ifndef __GAME_BUILD_H__