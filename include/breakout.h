#ifndef __BREAKOUT_H__
#define __BREAKOUT_H__

#include "bounce.h"
#include <math.h>
#include <stdlib.h>
#include "sprite.h"
#include "collision.h"
#include <stdio.h>
#include "forces.h"
#include <time.h>
#include <unistd.h>
#include "color.h"
#include "breakout.h"

const int SCREEN_SIZE_X;
const int SCREEN_SIZE_Y;
const vector_t min;
const vector_t max;

const double NUMBER_OF_BOXES_PER_ROW;
const double NUMBER_OF_BOXES_PER_COLUMN;
const double BOXES_RANGE_OF_COLORS;
const int BOXES_COLOR_SATURATION;
const int BOXES_COLOR_HUE;
const int Y_START_OF_BOXES;
const double ROW_SPACE_BETWEEN_BOXES;
const double COLUMN_SPACE_BETWEEN_BOXES;
const int BOX_MASS;

const int PADDLE_MIN_LENGTH;
const int PADDLE_MAX_LENGTH;
const int PADDLE_MASS;
const double PADDLE_X_POS;
const double PADDLE_Y_POS;
const vector_t PADDLE_VELOCITY;

const int BALL_RADIUS;
const double BALL_MASS;
const double BALL_ELASTICITY;
const vector_t BALL_VELOCITY;

const double VERTICAL_SPACE_OF_BOXES;
const double BALL_Y_POS;

const int PEG_RADIUS;
const int NUMBER_OF_PEGS;
const int PEG_MASS;

const double VELOCITY_BOOST_HOLD_KEY;

/**
 * Adds physics_collision_2 to the ball and a box in the scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball ball in the game
 * @param box the bricks at the top of the screen
 */
void ball_box_collision(scene_t *scene, body_t *ball, body_t *box);

/**
 * Adds physics_collision to a ball and peg in the scene. 
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball ball in the game
 * @param peg dots in the screen that changes the ball's velocity when
 *            they collide.
 */
void ball_peg_collision(scene_t *scene, body_t *ball, body_t *peg);

/**
 * Adds physics_collision to a ball and the paddle in the scene. 
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball ball in the game
 * @param paddle the rectangle at the bottom that reflects the ball
 */
void ball_paddle_collision(scene_t *scene, body_t *ball, body_t *paddle);

/**
 * Creates the boxes at the top of the screen.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball ball in the game
 */
void create_boxes(scene_t *scene, body_t *ball);

/**
 * Makes the defender wrap around the screen
 *
 * @param original_position current position of the defender
 */
vector_t wrap_position(vector_t original_position);

/**
 * Moves the paddle left and right
 *
 * @param dx scale of how far the defender should move
 * @param paddle rectangle at the bottom that moves
 */
void move_paddle(double dx, body_t *paddle);

/**
 * Reflects the ball off the sides of the screen.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball ball in the game
 */
void reflect_ball(scene_t *scene, body_t *ball);

/**
 * Uses keys to determine actions for the paddle
 *
 * @param key charater representing the specific key pressed
 * @param type the type of event being considered
 * @param held_time the amount of time the user has been holding the key
 * @param paddle rectangle at the bottom that moves
 */
void handle(char key, key_event_type_t type, double held_time, body_t *paddle);

/**
 * Creates the rectangle at the bottom of the screen.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * 
 * Returns: the body_t of the paddle
 */
body_t *make_paddle(scene_t *scene);

/**
 * Creates the ball in the game.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * 
 * Returns: the body_t of the ball
 */
body_t *create_ball(scene_t *scene);

/**
 * Determines whether the game should be restarted.
 *
 * @param ball ball in the game
 * 
 * Returns: boolean
 * If the ball goes out of the screen, return true. 
 * Otherwise return false
 */
bool restart_game(body_t *ball);

/**
 * Created the pegs in random spots on the screen.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball ball in the game
 */
void create_pegs(scene_t *scene, body_t *ball);

/**
 * Clears all the objects on the screen.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void clear_scene(scene_t *scene);

/**
 * Creates new boxes and pegs to add to the screen
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param ball the bullets that are created from the defender
 * @param paddle rectangle at the bottom that moves
 */
void create_new_game(scene_t *scene, body_t *ball, body_t *paddle);

#endif // #ifndef __BREAKOUT_H__