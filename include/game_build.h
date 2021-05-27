#ifndef __GAME_BUILD_H__
#define __GAME_BUILD_H__

#include "body.h"
#include "collision.h"
#include "color.h"
#include "forces.h"
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "collision.h"
#include "spacelaunch2.h"

const int GB_SCREEN_SIZE_X;
const int GB_SCREEN_SIZE_Y;

const int GB_MAX_OBSTACLES_SCREEN_SIZE_X;
const int GB_MAX_OBSTACLES_SCREEN_SIZE_Y;

const double GB_ASTEROID_MASS;
const rgb_color_t GB_BAD_ASTEROID_COLOR;
const rgb_color_t GB_GOOD_ASTEROID_COLOR;
const int GB_INITIAL_ASTEROIDS;
const int GB_ASTEROID_RADIUS;

const rgb_color_t GB_BACKGROUND_COLOR;

const int GB_DISTANCE_BETWEEN_STARS;
const double GB_FREQUENCY_FOR_STARS;
const rgb_color_t GB_STAR_COLOR;
const int GB_STAR_NUM_OF_POINTS;
const int GB_STAR_MIN_LENGTH;
const int GB_STAR_MAX_LENGTH;

const vector_t GB_ROCKET_INITIAL_POS;
const int GB_ROCKET_RADIUS;
const double GB_ROCKET_MASS;
const rgb_color_t GB_ROCKET_COLOR;

const int GB_SHOOTING_STAR_RADIUS;
const int GB_SHOOTING_STAR_MASS;
const rgb_color_t GB_SHOOTING_STAR_COLOR;
const vector_t GB_SHOOTING_STAR_VELCOITY;

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
void game_build_draw_asteroids(scene_t *scene, body_t *rocket);

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
 * @return pointer to the rocket
 */
body_t *game_build_rocket(scene_t *scene);

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
 * Makes the asteroids.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param rocket a pointer to the rocket in the scene.
 */
void game_build_asteroid(scene_t *scene, body_t *rocket);

#endif // #ifndef __GAME_BUILD_H__