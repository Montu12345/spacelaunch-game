#ifndef __BODY_H__
#define __BODY_H__

#include "color.h"
#include "list.h"
#include "vector.h"
#include "polygon.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

extern const double BODY_DEFAULT_ANGULAR_VELOCITY;
extern const double BODY_DEFAULT_ANGULAR_POSITION;
extern const double BODY_DEFAULT_BOUNCINESS;
extern const double BODY_DEFAULT_VELOCITY_X;
extern const double BODY_DEFAULT_VELOCITY_Y;
extern const bool BODY_IS_MOVABLE;

/**
 * Visual properties of a body
 *  shape -- the polygon representing the body's verticies
 *  color -- the rgb color of the body
*/
typedef struct body_appearance body_appearance_t;

/**
 * Intrinsic physical quantities describing the body
 *  mass -- the gravity-independent weight of the body 
 *  bounciness -- used to compute coefficient of restitution for collisions
 *  movable -- true if a colliding object has the capacity to move the body.
*/
typedef struct body_physical_properties body_physical_properties_t;

typedef char *(*texture_path_func_t)(void *);

/**
 * Properties describing the motion of the body
 *  position -- vector coordinate the object is placed upon
 *  velocity -- vector describing change in position
 *  angular_velocity -- amount of rotation per unit time
 *  net_force -- vector proportional to body's velocity change
*/
typedef struct body_kinematic_variables body_kinematic_variables_t;

/**
 * Helpful attributes for managing the physics engine
 *  unique_id -- tag used for checking equality of bodys
 *  collision_flag -- the unique_id, if any, the body is colliding with
*/
typedef struct body_descriptors body_descriptors_t;

/**
 * A rigid body constrained to the plane.
 * Implemented as a polygon with uniform density.
 * Bodies can accumulate forces and impulses during each tick.
 * Angular physics (i.e. torques) are not currently implemented.
 */
typedef struct body body_t;

/**
 * Categories for rules for each body to follow in regards to the camera.
 * Each camera mode is customizable, these are the intended functionalities.
 */
typedef enum
{
    LOCKED,     // Body is not affected by the camera
    BACKGROUND, // For creating moving backgrounds
    FOREGROUND, // For creating moving foregrounds
    FOLLOW,     // Subject of camera movement, used once.
    SCENE       // Elements of the scene which should be left behind
} camera_mode_t;

/**
 * Allocates memory for a body with the given parameters.
 * The body is initially at rest.
 * Asserts that the mass is positive and that the required memory is allocated.
 *
 * @param shape a list of vectors describing the initial shape of the body
 * @param mass the mass of the body (if INFINITY, stops the body from moving)
 * @param color the color of the body, used to draw it on the screen
 * @return a pointer to the newly allocated body
 */
body_t *body_init(list_t *shape, double mass, rgb_color_t color);

/**
 * Allocates memory for a body with the given parameters.
 * The body is initially at rest.
 * Asserts that the mass is positive and that the required memory is allocated.
 *
 * @param shape a list of vectors describing the initial shape of the body
 * @param mass the mass of the body (if INFINITY, stops the body from moving)
 * @param color the color of the body, used to draw it on the screen
 * @param info additional information to associate with the body,
 *   e.g. its type if the scene has multiple types of bodies
 * @param info_freer if non-NULL, a function call on the info to free it
 * @return a pointer to the newly allocated body
 */
body_t *body_init_with_info(
    list_t *shape,
    double mass,
    rgb_color_t color,
    void *info,
    free_func_t info_freer);

/**
 * Gets the information associated with a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the info passed to body_init()
 */
void *body_get_info(body_t *body);

/**
 * Marks a body for removal--future calls to body_is_removed() will return true.
 * Does not free the body.
 * If the body is already marked for removal, does nothing.
 *
 * @param body the body to mark for removal
 */
void body_remove(body_t *body);

/**
 * Returns whether a body has been marked for removal.
 * This function returns false until body_remove() is called on the body,
 * and returns true afterwards.
 *
 * @param body the body to check
 * @return whether body_remove() has been called on the body
 */
bool body_is_removed(body_t *body);

/**
 * Releases the memory allocated for a body.
 *
 * @param body a pointer to a body returned from body_init()
 */
void body_free(body_t *body);

/**
 * Gets the current shape of a body.
 * Returns a newly allocated vector list, which must be list_free()d.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the polygon describing the body's current position
 */
list_t *body_get_shape(body_t *body);

/**
 * Get the path to the image displayed as the body.
 * 
 * @param body a pointer to a body returned from body_init()
 * @return the path to the body's image resource
 */
char *body_get_texture_path(body_t *body);

/**
 * Set the path to the image displayed as the body.
 * 
 * @param body a pointer to a body returned from body_init()
 * @param path the path to the body's image resource
 */
void body_set_static_texture_path(body_t *body, char *path);

/**
 * Set a function to be called to return the current texture for the body.
 * Is useful for animations. 
 * 
 * @param body a pointer to a body returned from body_init()
 * @param path_func texture_path_func_t to return the path given an aux variable
 * @param aux information required by the path_func, if necessary
 * @param freer free_func_t to free the aux
 */
void body_set_texture_path_func(body_t *body, texture_path_func_t path_func, void *aux, free_func_t freer);

/**
 * Gets the rectangle which the body's shape is bounded by.
 * The bounds are determined by the minimum and maximum x and
 * y positions of the shape's verticies. 
 * 
 * @param body the body to extract the shape from 
 * @return pointer to an allocated SDL_Rect representing the bounds
 */
SDL_Rect *body_get_bounding_rect(body_t *body);

/**
 * Gets the current center of mass of a body.
 * While this could be calculated with polygon_centroid(), that becomes too slow
 * when this function is called thousands of times every tick.
 * Instead, the body should store its current centroid.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's center of mass
 */
vector_t body_get_centroid(body_t *body);

/**
 * Gets the current velocity of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's velocity vector
 */
vector_t body_get_velocity(body_t *body);

/**
 * Gets the current angular velocity of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's angular velocity
 */
double body_get_angular_velocity(body_t *body);

/**
 * Gets the current angular position of a body. Represents
 * the amount the body is rotated counterclockwise.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the body's angular position 
 */
double body_get_rotation(body_t *body);

/**
 * Gets the mass of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the mass passed to body_init(), which must be greater than 0
 */
double body_get_mass(body_t *body);

/**
 * Gets the display color of a body.
 *
 * @param body a pointer to a body returned from body_init()
 * @return the color passed to body_init(), as an (R, G, B) tuple
 */
rgb_color_t body_get_color(body_t *body);

/**
 * Translates a body to a new position.
 * The position is specified by the position of the body's center of mass.
 *
 * @param body a pointer to a body returned from body_init()
 * @param x the body's new centroid
 */
void body_set_centroid(body_t *body, vector_t x);

/**
 * Change'e the color of the body.
 *
 * @param body a pointer to a body returned from body_init()
 * @param color the body's new color
 */
void body_set_color(body_t *body, rgb_color_t color);

/**
 * Changes a body's velocity (the time-derivative of its position).
 *
 * @param body a pointer to a body returned from body_init()
 * @param v the body's new velocity
 */
void body_set_velocity(body_t *body, vector_t v);

/**
 * Changes a body's angular velocity (the time-derivative of its rotation).
 *
 * @param body a pointer to a body returned from body_init()
 * @param av the body's new angular velocity
 */
void body_set_angular_velocity(body_t *body, double av);

/**
 * Changes a body's capacity to move
 *
 * @param body a pointer to a body returned from body_init()
 * @param movable a boolean for whether or not the body should be movable
 */
void body_set_movable(body_t *body, bool movable);

/**
 * Checks the property storing body's capacity to move 
 *
 * @param body a pointer to a body returned from body_init()
 * @return boolean, true if the body is movable
 */
bool body_is_movable(body_t *body);

/**
 * Changes a body's tracking by the camera 
 *
 * @param body a pointer to a body returned from body_init()
 * @param camera_mode the type of desired tracking
 */
void body_set_camera_mode(body_t *body, camera_mode_t camera_mode);

/**
 * Gets the camera mode of the body
 *
 * @param body a pointer to a body returned from body_init()
 * @return camera_mode_t, the body's set camera mode
 */
camera_mode_t body_get_camera_mode(body_t *body);

/**
 * Changes a body's orientation in the plane.
 * The body is rotated about its center of mass.
 * Note that the angle is *absolute*, not relative to the current orientation.
 *
 * @param body a pointer to a body returned from body_init()
 * @param angle the body's new angle in radians. Positive is counterclockwise.
 */
void body_set_rotation(body_t *body, double angle);

/**
 * Applies a force to a body over the current tick.
 * If multiple forces are applied in the same tick, they should be added.
 * Should not change the body's position or velocity; see body_tick().
 *
 * @param body a pointer to a body returned from body_init()
 * @param force the force vector to apply
 */
void body_add_force(body_t *body, vector_t force);

/**
 * Applies an impulse to a body.
 * An impulse causes an instantaneous change in velocity,
 * which is useful for modeling collisions.
 * If multiple impulses are applied in the same tick, they should be added.
 * Should not change the body's position or velocity; see body_tick().
 *
 * @param body a pointer to a body returned from body_init()
 * @param impulse the impulse vector to apply
 */
void body_add_impulse(body_t *body, vector_t impulse);

/**
 * Determines if the body is experiencing a net impulse.
 * 
 * @param body a pointer to a body returned from body_init()
 * @return bool, true if the body is under an impulse.
 */
bool body_has_impulse(body_t *body);


/**
 * Returns the body's force.
 *
 * @param body a pointer to a body returned from body_init()
 */
vector_t body_get_force(body_t *body);
/**
 * Updates the body after a given time interval has elapsed.
 * Sets acceleration and velocity according to the forces and impulses
 * applied to the body during the tick.
 * The body should be translated at the *average* of the velocities before
 * and after the tick.
 * Resets the forces and impulses accumulated on the body.
 *
 * @param body the body to tick
 * @param dt the number of seconds elapsed since the last tick
 */
void body_tick(body_t *body, double dt);

/**
 * Moves the body with respect to the camera. 
 * To be called each tick.
 * @param body the body to adjust according to the camera.
 * @param movement the result of the camera_mover_func_t on offset
 */
void body_adjust_for_camera(body_t *body, vector_t movement);

void body_set_type(body_t *body, void* type);

#endif // #ifndef __BODY_H__
