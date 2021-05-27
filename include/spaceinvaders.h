#ifndef __SPACEINVADERS_H__
#define __SPACEINVADERS_H__

#include "bounce.h"
#include <math.h>
#include <stdlib.h>
#include "sprite.h"
#include "collision.h"
#include <stdio.h>
#include "forces.h"
#include <time.h>
#include <unistd.h>

extern const int SCREEN_SIZE_X;
extern const int SCREEN_SIZE_Y;
extern const vector_t min;
extern const vector_t max;

extern const int VEL_X;
extern const double MIN_STAR_LENGTH;
extern const double MAX_STAR_LENGTH;
extern const vector_t INVADERS_VELOCITY;
extern const vector_t DEFENDERS_VELOCITY;
extern const double INVADER_RADIUS;
extern const double INVADER_BOTTOM_DEPTH;
extern const int DEFENDER_MIN_LENGTH;
extern const int DEFENDER_MAX_LENGTH;
extern const int DEFENDER_MASS;
extern const int INVADER_MASS;
extern const vector_t DEFENDER_BULLET_VELOCITY;
extern const vector_t INVADER_BULLET_VELOCITY;
extern const rgb_color_t BULLET_COLOR;
extern const int BULLET_MASS;
extern const int BULLET_WIDTH;
extern const int BULLET_HEIGHT;
extern const int INVADERS_PER_ROW;
extern const int INVADERS_PER_COLUMN;
extern const double DEFENDER_X_POS;
extern const double DEFENDER_Y_POS;
extern const int FREQUENCY_INVADER_SHOTS;
extern const double VELOCITY_BOOST_HOLD_KEY;
/**
 * Values to pass into handler.
 *  defender -- the defender
 *  demo -- the scene
*/
typedef struct space_aux_t;

space_aux_t space_aux_init(
    body_t *defender,
    scene_t *demo);

/**
 * Adds destructive collision to a bullet and the invaders in the scene. 
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param bullets the bullets that are created from the defender
 */
void bullet_invader_collision(scene_t *scene, body_t *bullets);

/**
 * Adds destructive collision to a bullet and the defender in the scene. 
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param bullets the bullet that is created
 * @param defender the defender in the scene
 */
void bullet_defender_collision(scene_t *scene, body_t *bullets, body_t *defender);

/**
 * Adds destructive collision to the invaders and the defender in the scene. 
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param defender the defender in the scene
 * @param invader an invader in the scene
 */
void invader_defender_collision(scene_t *scene, body_t *invader, body_t *defender);

/**
 * Creates the bullets that the defender shoots.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param defender the defender in the scene
 */
void make_defender_bullets(scene_t *scene, body_t *defender);

/**
 * Creates the bullets that the invaders shoot
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param defender the defender in the scene
 * @param invader an invader in the scene
 */
void make_invaders_bullets(scene_t *scene, body_t *invader, body_t *defender);

/**
 * Creates the invaders in the scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param defender the defender in the scene
 */
void make_invaders(scene_t *scene, body_t *defender);

/**
 * Moves the invaders down when they hit an edge.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void teleport_invaders(scene_t *scene);

/**
 * Makes the defender wrap around the screen
 *
 * @param original_position current position of the defender
 */
vector_t wrap_position(vector_t original_position);

/**
 * Moves the defender left and right
 *
 * @param dx scale of how far the defender should move
 * @param defender the defender in the scene
 */
void move_defender(double dx, body_t *defender);

/**
 * Handles the key presses:
 * Spacebar = defender shoots a bullet
 * Left/Right = direction of defender's movement
 *
 * @param key charater representing the specific key pressed
 * @param type the type of event being considered
 * @param held_time the amount of time the user has been holding the key
 * @param aux storage of the scene and defender
 */
void handle(char key, key_event_type_t type, double held_time, space_aux_t *aux);

/**
 * Makes the defender.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param dt the time elapsed since the last tick, in seconds
 */
body_t *make_defender(scene_t *scene);

/**
 * Randomly makes bullets that random invaders shoot after a certain 
 * amount of ticks.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param defender the defender in the scene
 * @param i counter of number of ticks
 */
int invaders_shoot(scene_t *scene, body_t *defender, int i);

#endif // #ifndef __SPACEINVADERS_H__