#include "sprite.h"
#include "collision.h"
#include "forces.h"

const double ROCKET_ELASTICITY = 2.0;

enum body_type_t
{
    obstacle_t,
    planet_t,
    rocket_t
};

enum body_type_t *body_type_init(enum body_type_t b)
{
    enum body_type_t *body_type = malloc(sizeof(enum body_type_t));
    *body_type = b;
    return body_type;
}

void create_gravity_rocket_obstacles(scene_t *scene, body_t *rocket, body_t *obstacle){
    create_physics_collision(scene, ROCKET_ELASTICITY, rocket, obstacle);
}