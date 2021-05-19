#ifndef __DAMPING_H__
#define __DAMPING_H__

#include "scene.h"
#include "body.h"
#include "list.h"
#include "sprite.h"
#include "color.h"
#include "sdl_wrapper.h"

extern const int SCREEN_SIZE_X;
extern const int SCREEN_SIZE_Y;

extern const int BALL_RADIUS;
extern const int BALL_MASS;
const int ANCHOR_RADIUS;
const int ANCHOR_MASS;
const int HOOKES_CONSTANT;
const int MAX_Y_IMPULSE;

const vector_t MIN;
const vector_t MAX;
const rgb_color_t ANCHOR_COLOR;

/**
 * Makes and anchor for each circle on the screen.
 * Has a white color to match background.
 *
 * @param scene a pointer to a scene initialized in main
 * @param idx the index of the anchor in a set of anchors
 */
void make_anchor(scene_t *scene, int idx);

/**
 * Creates the balls.
 *
 * @param scene a pointer to a scene initialized in main
 */
void make_bodies(scene_t *scene);

/**
 * Applies the spring force and inpulse to the balls.
 *
 * @param scene a pointer to a scene initialized in main
 */
void move_bodies(scene_t *scene);

#endif // #ifndef __DAMPING_H__