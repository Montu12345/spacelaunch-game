/** @file sprite.h
 *  @brief Function prototypes for generating polygonal shapes.
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#ifndef __SPRITE_H__
#define __SPRITE_H__

#include "list.h"
#include <math.h>

extern const size_t SPRITE_CIRCLE_POINTS_PER_RADIAN;
extern const double SPRITE_PACMAN_MOUTH_ANGLE;

/** @brief Computes the vertices of a n-sided star.
 *
 * @param num_points the number of points of the polygon
 * @return a list of vectors of the star
*/
list_t *sprite_make_star(int num_points, double min_star_length, double max_star_length);

/** @brief Computes the vertices of a rectangle.
 *
 * @param min_x lowest x-position of the rectangle
 * @param max_x greatest x-position of the rectangle
 * @param min_y lowest y-position of the rectangle
 * @param max_y greatest y-position of the rectangle
 * @return a list of vectors of the rectangle
*/
list_t *sprite_make_rect(double min_x, double max_x, double min_y, double max_y);

/** @brief Adds a point at a specified radius and angle to the sprite
 *
 * @param sprite the list of points in a sprite
 * @param radius the distance from the origin of the point
 * @param angle the angle from the x-axis of the point
 * @return none
*/
void sprite_add_radial_point(list_t *sprite, double radius, double angle);

/** @brief Adds an arc of points according to the supplied start and 
 *  end angles and SPRITE_CIRCLE_POINTS_PER_RADIAN. Adds points 
 *  in a counterclockwise direction.
 *
 * @param sprite the list of points in a sprite
 * @param radius the distance from the origin of all points
 * @param start_angle the first angle on the arc to place a point
 * @param end_angle the final angle on the arc to place a point
 * @return none
*/
void sprite_add_arc(list_t *sprite, double radius, double start_angle, double end_angle);

/** @brief Makes an ellipse centered at the origin, intersecting the x and y axes at the specified points
 * 
 * Uses the polar solution to the elipse equation (x/a)^2 + (y/b)^2 = 1
 *
 * @param x_axis_radius intersection between the ellipse and the x-axis
 * @param y_axis_radius intersection between the ellipse and the y-axis
 * @return list_t the points on the ellipse
*/
list_t *sprite_make_ellipse(double x_axis_radius, double y_axis_radius);

/** @brief Makes an arc with a pointy bottom to mimic an invader from the game Space Invaders.
 *
 * @param radius radius of the arc at the top of the invader
 * @param bottom_depth amount at which the bottom of the invader should stick out
 * @return list_t the points on the space invader
*/
list_t *sprite_make_invader(double radius, double bottom_depth);

/** @brief For a specified radius, adds points in a long arc
 *  to make a shape that looks like the video game character Pac-Man.
 *  Adds points counterclockwise, starting with a point at the origin 
 *  for the inside of Pac-Man's mouth.
 * 
 * @param radius the distance from the origin of Pac-Man's arc
 * @return list of vectors representing points on Pac-Man
*/
list_t *sprite_make_pacman(double radius);

/** @brief For a specified radius, adds points in an arc in a
 *  clockwise direction starting at the origin to compose a circle.
 * 
 * @param radius the distance from the origin of each point
 * @return list of vectors representing points on the circle
*/
list_t *sprite_make_circle(double radius);

#endif // #ifndef __SPRITE_H__
