#ifndef __NBODIES_H__
#define __NBODIES_H__
#include "scene.h"
#include "body.h"
#include "list.h"
#include "sprite.h"
#include "color.h"
#include "sdl_wrapper.h"
#include <time.h>
#include "forces.h"

/** @file nbodies.h
 *  @brief Function implementations for an "n-bodies" animation
 *
 *  This contains the function implementations for multiple stars
 *  being pulled by gravity animation.
 *
 *  @author Alexis Wang, Ian Fowler, Ezra Johnson, Shevali Kadakia
 *  @bug No known bugs.
 */

// The screen size
const int SCREEN_SIZE_X;
const int SCREEN_SIZE_Y;

// The properties of the stars
const int STAR_MIN_LENGTH;
const int STAR_MAX_LENGTH;
const double BALL_MASS;
const int NUMBER_OF_STARS;
const int MAX_VELCOITY;
const double GRAVITY_CONSTANT;

// The extrema of the screen size
const vector_t min;
const vector_t max;

/**
 * @brief Creates NUMBER_OF_STARS of stars and adds them to the scene.
 * 
 * @param scene scene
 * @return none
 */
void make_bodies(scene_t *scene);

/**
 * @brief Moves the stars by adding Newtonian gravity.
 * 
 * @param scene scene
 * @return none
 */
void add_gravity(scene_t *scene);

#endif // #ifndef __NBODIES_H__