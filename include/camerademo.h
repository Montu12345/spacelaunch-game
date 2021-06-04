#ifndef __CAMERADEMO_H__
#define __CAMERADEMO_H__
#include "scene.h"
#include "sdl_wrapper.h"
#include "sprite.h"

/** @file pacman.h
 *  @brief Function implementations for a pacman animation
 *
 *  This contains the function implementations for a pacman
 *  eating stuff animation.
 *
 *  @author Alexis Wang, Ian Fowler, Ezra Johnson, Shevali Kadakia
 *  @bug No known bugs.
 */

extern const int SCREEN_SIZE_X;
extern const int SCREEN_SIZE_Y;

// The extrema of the screen dimensions
extern const vector_t min;
extern const vector_t max;

// The number of screen units Pac-Man moves per key press
extern const int PACMAN_STEP;

// The coefficient to the timestep used to adjust velocity
extern const double PACMAN_VELOCITY_SCALE;

// Number of points to draw for
extern const int CIRCLE_PRECISION;
extern const int PACMAN_RADIUS;

// The distance the center of pac
extern const double EAT_TOLERANCE;

// The number of dots to keep on the screen
extern const int INITIAL_DOTS;
extern const double DOT_ADD_PERIOD;

// Color for game bodies
extern const rgb_color_t PACMAN_COLOR;
extern const rgb_color_t DOT_COLOR;

/**
 * @brief Make pacman jump to the other side of the SCREEN_SIZE_X
 *
 * @param original_position where Pac-Man is currently located
 * @return vector_t the new position, same if not wrapped.
 */
extern vector_t wrap_position(vector_t original_position);

/**
 * @brief Make pacman eat the dot if Pac-Man's centroid is within the
 * EAT_TOLERANCE.
 *
 * @param dot_idx the index in the scene where dot is located
 * @param dot the dot to consider consumption by Pac-Man
 * @param pacman Pac-Man body
 * @param scene scene
 */
bool eat(size_t dot_idx, body_t *dot, body_t *pacman, scene_t *scene);

/**
 * @brief Pass all dots to the eat function, removing the dots
 * Pac-Man is currently eating
 *
 * @param pacman Pac-Man body
 * @param scene scene
 * @return none
 */
void check_eat(body_t *pacman, scene_t *scene);

/**
 * @brief Rotates pacman the the relevant angle and moves him on the
 * screen to follow the direction of the new movement.
 * @param angle the angle relative to the x-axis where Pac-Man should move
 * @param scale a factor by which to multiply the movement of Pac-Man
 * @return none
 */
void move_pacman(double angle, double scale, body_t *pacman);

/**
 * @brief Handler for a key press event. Maps the arrow keys to
 * Pac-Man's potential movements.
 *
 * @param key charater representing the specific key pressed
 * @param type the type of event being considered
 * @param held_time the amount of time the user has been holding the key
 * @param body_t a body that moves
 * @return none
 */
void handle(char key, key_event_type_t type, double held_time, body_t *pacman);

/**
 * @brief Adds a dot to the scene for Pac-Man to feast upon.
 *
 * @param scene scene
 * @return none
 */
void make_dot(scene_t *scene, body_t *pacman);

#endif // #ifndef __CAMERADEMO_H__
