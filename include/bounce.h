/** @file bounce.h
 *  @brief Function prototypes for a bounce animation
 *
 *  This contains the function prototypes for a polygon bouncing
 *  animation.
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#ifndef __BOUNCE_H__
#define __BOUNCE_H__

#include "color.h"
#include "list.h"
#include "polygon.h"
#include "sdl_wrapper.h"
#include "sprite.h"
#include "vector.h"

extern const int SCREEN_SIZE_X;
extern const int SCREEN_SIZE_Y;
extern const double ANGLE;
extern const int INITIAL_VELOCITY_X;
extern const int INITIAL_VELOCITY_Y;
extern const double MIN_STAR_LENGTH;
extern const double MAX_STAR_LENGTH;
extern const rgb_color_t STAR_COLOR;

/** @brief Computes the area of a polygon.
 *
 *  @param object the polygon
 *  @param dt the frame rate
 *  @param velocity the
 *  @return None
 */
void compute_new_positions(list_t *object, double dt, vector_t *velocity);

/** @brief Computes the vertices of a n-sided star.
 *
 * @param num_points the number of points of the polygon
 * @return a list of vectors of the star
 */
list_t *make_star(int num_points);

#endif