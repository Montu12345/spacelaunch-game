#ifndef __SPACELAUNCH_H__
#define __SPACELAUNCH_H__

#include "sprite.h"
#include "collision.h"
#include "forces.h"

const double ROCKET_ELASTICITY;

/**
 * Created gravity between the rocket and the obstacles.
 * The rocket changes velocity when it hits an obstacle.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param rocket the rocket
 * @param obstacle the obstacle that makes the rocket change its velocity
 */
void create_gravity_rocket_obstacles(scene_t *scene, body_t *rocket, body_t *obstacle);

#endif // #ifndef __SPACELAUNCH_H__