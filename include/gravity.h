#include "pacman.h"
#include "scene.h"
#include "sprite.h"
#include "sdl_wrapper.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>

/** @file gravity.h
 *  @brief Function implementations for a gravity animation
 *
 *  This contains the function implementations for a star
 *  bouncing off the ground animation.
 *
 *  @author Alexis Wang, Ian Fowler, Ezra Johnson, Shevali Kadakia
 *  @bug No known bugs.
 */
extern const int SCREEN_SIZE_X;
extern const int SCREEN_SIZE_Y;

extern const int STAR_NUM_POINTS;
extern const double STAR_MIN_LENGTH;
extern const double STAR_MAX_LENGTH;
extern const double STAR_MASS;
extern const double COEFFICIENT_OF_RESTITUTION;
extern const double STAR_ADD_PERIOD;

extern const rgb_color_t GROUND_COLOR;

extern const double INITIAL_VELOCITY_X;
extern const double INITIAL_VELOCITY_Y;
extern const double ANGULAR_VELOCITY;

extern const double INITIAL_POSITION_X;
extern const double INITIAL_POSITION_Y;

extern const vector_t ACCELERATION;

extern const double GROUND_HEIGHT;
extern const double SCENE_RIGHT_BOUDNARY;

extern const vector_t min;
extern const vector_t max;

extern scene_t *scene;

/** @brief Generates a star to add to the gravity demo
 *  
 *  @param num_points number of points in the star
 *  @return none
 */
void add_star(size_t num_points);

/** @brief Generates a rectangle to add to the gravity demo
 *  It looks like the ground.
 *  
 *  @return none
 */
void add_ground();

/** @brief Takes out a body if it's outside the parameters
 *  
 *  @param body the body to consider the position relative to the boundary
 *  @param idx the index of the body in the scene
 *  @return boolean, true if the body has escaped 
 */
bool remove_body_if_escaped(body_t *body, size_t idx);

/** @brief Changes the velocity of a body downward
 *  
 *  @param body the body to consider the position relative to the boundary
 *  @param dt the change in time since the last displayed frame
 *  @return none
 */
void accelerate_body(body_t *body, double dt);

/** @brief True if the body is currently touching the ground
 *  
 *  @param body the body to consider position relative to the ground
 *  @return boolean, true if the body has hit the ground visually
 */
bool body_touches_ground(body_t *body);

/** @brief Bounces a body off of the ground if applicable
 *  
 *  @param body the body to consider to bounce off of the ground
 *  @return none
 */
void bounce_body(body_t *body);

/** @brief Moves all of the bodies on the screen 
 *  
 *  @param dt the time since the last frame displayed 
 *  @return none
 */
void update_bodies(double dt);
