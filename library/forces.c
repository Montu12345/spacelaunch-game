/** @file forces.c
 *  @brief Implementations for force functionality
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#include "forces.h"
#include "body.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

const double GRAVITY_MIN_DISTANCE = 50.0;

typedef struct gravity_params {
  double G;
  body_t *body1;
  body_t *body2;
} gravity_params_t;

typedef struct spring_params {
  double k;
  body_t *body1;
  body_t *body2;
} spring_params_t;

typedef struct drag_params {
  double gamma;
  body_t *body;
} drag_params_t;

typedef struct destructive_params {
  body_t *body1;
  body_t *body2;
} destructive_params_t;

typedef struct collision_event_params {
  body_t *body1;
  body_t *body2;
  collision_handler_t handler;
  void *aux;
  free_func_t freer;
  bool was_colliding;
} collision_event_params_t;

gravity_params_t *gravity_params_init(double G, body_t *body1, body_t *body2) {
  gravity_params_t *params = malloc(sizeof(gravity_params_t));
  *params = (gravity_params_t){.G = G, .body1 = body1, .body2 = body2};
  return params;
}

spring_params_t *spring_params_init(double k, body_t *body1, body_t *body2) {
  spring_params_t *params = malloc(sizeof(spring_params_t));
  *params = (spring_params_t){.k = k, .body1 = body1, .body2 = body2};
  return params;
}

drag_params_t *drag_params_init(double gamma, body_t *body) {
  drag_params_t *params = malloc(sizeof(drag_params_t));
  *params = (drag_params_t){.gamma = gamma, .body = body};
  return params;
}

destructive_params_t *destructive_params_init(body_t *body1, body_t *body2) {
  destructive_params_t *params = malloc(sizeof(destructive_params_t));
  *params = (destructive_params_t){.body1 = body1, .body2 = body2};
  return params;
}

collision_event_params_t *
collision_event_params_init(body_t *body1, body_t *body2,
                            collision_handler_t handler, void *aux,
                            free_func_t freer, bool was_colliding) {
  collision_event_params_t *params = malloc(sizeof(collision_event_params_t));
  *params = (collision_event_params_t){.body1 = body1,
                                       .body2 = body2,
                                       .handler = handler,
                                       .aux = aux,
                                       .freer = freer,
                                       .was_colliding = was_colliding};
  return params;
}

void collision_event_params_free(collision_event_params_t *params) {
  if (params->freer != NULL) {
    params->freer(params->aux);
  }
  free(params);
}

list_t *get_bodies_list(body_t *body1, body_t *body2) {
  list_t *bodies = list_init(2, NULL);
  list_add(bodies, body1);
  list_add(bodies, body2);
  return bodies;
}

void gravity_forcer(gravity_params_t *params) {
  vector_t pos1 = body_get_centroid(params->body1);
  vector_t pos2 = body_get_centroid(params->body2);
  double m1 = body_get_mass(params->body1);
  double m2 = body_get_mass(params->body2);
  vector_t dist_from_2_to_1 = vec_subtract(pos1, pos2);
  vector_t direction_from_2_to_1 = vec_unit(dist_from_2_to_1);
  double distance = vec_magnitude(dist_from_2_to_1);
  double grav_magnitude = params->G * m1 * m2 / (distance * distance);
  vector_t F_from_2_to_1 = vec_multiply(grav_magnitude, direction_from_2_to_1);
  vector_t F_from_1_to_2 = vec_negate(F_from_2_to_1);

  if (distance > GRAVITY_MIN_DISTANCE) {
    body_add_force(params->body1, F_from_1_to_2);
    body_add_force(params->body2, F_from_2_to_1);
  }
}

void create_newtonian_gravity(scene_t *scene, double G, body_t *body1,
                              body_t *body2) {
  gravity_params_t *params = gravity_params_init(G, body1, body2);
  list_t *bodies = get_bodies_list(body1, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)gravity_forcer, params,
                                 bodies, free);
}

void spring_forcer(spring_params_t *params) {
  vector_t pos1 = body_get_centroid(params->body1);
  vector_t pos2 = body_get_centroid(params->body2);
  vector_t dist_from_2_to_1 = vec_subtract(pos1, pos2);
  vector_t F_from_2_to_1 = vec_multiply(params->k, dist_from_2_to_1);
  vector_t F_from_1_to_2 = vec_negate(F_from_2_to_1);
  body_add_force(params->body1, F_from_1_to_2);
  body_add_force(params->body2, F_from_2_to_1);
}

void create_spring(scene_t *scene, double k, body_t *body1, body_t *body2) {
  spring_params_t *params = spring_params_init(k, body1, body2);
  list_t *bodies = get_bodies_list(body1, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)spring_forcer, params,
                                 bodies, free);
}

void drag_forcer(drag_params_t *params) {
  vector_t velocity = body_get_velocity(params->body);
  vector_t drag = vec_multiply(-1 * params->gamma, velocity);
  body_add_force(params->body, drag);
}

void create_drag(scene_t *scene, double gamma, body_t *body) {
  drag_params_t *params = drag_params_init(gamma, body);
  list_t *bodies = list_init(1, NULL);
  list_add(bodies, body);
  scene_add_bodies_force_creator(scene, (force_creator_t)drag_forcer, params,
                                 bodies, free);
}

void destructive_forcer(body_t *body1, body_t *body2, vector_t axis,
                        void *aux) {
  body_remove(body1);
  body_remove(body2);
}

void create_destructive_collision(scene_t *scene, body_t *body1,
                                  body_t *body2) {
  create_collision(scene, body1, body2, (collision_handler_t)destructive_forcer,
                   NULL, NULL);
}

void collision_event_forcer(collision_event_params_t *params) {
  list_t *shape1 = body_get_shape(params->body1);
  list_t *shape2 = body_get_shape(params->body2);
  collision_info_t collision = find_collision(shape1, shape2);

  if (collision.collided && !params->was_colliding) {
    params->handler(params->body1, params->body2, collision.axis, params->aux);
    params->was_colliding = true;
  }
  params->was_colliding = collision.collided;
  list_free(shape1);
  list_free(shape2);
}

void create_collision(scene_t *scene, body_t *body1, body_t *body2,
                      collision_handler_t handler, void *aux,
                      free_func_t freer) {
  collision_event_params_t *params =
      collision_event_params_init(body1, body2, handler, aux, freer, false);
  list_t *bodies = get_bodies_list(body1, body2);
  scene_add_bodies_force_creator(scene, (force_creator_t)collision_event_forcer,
                                 params, bodies,
                                 (free_func_t)collision_event_params_free);
}

double reduced_mass(body_t *body1, body_t *body2) {
  double m1 = body_get_mass(body1);
  double m2 = body_get_mass(body2);
  if (m2 == INFINITY) {
    return m1;
  } else if (m1 == INFINITY) {
    return m2;
  }
  return m1 * m2 / (m1 + m2);
}

double velocity_along_axis(body_t *body, vector_t axis) {
  vector_t velocity = body_get_velocity(body);
  return vec_dot(velocity, axis);
}

vector_t impulse_to_body_1(body_t *body1, body_t *body2, vector_t axis,
                           double elasticity) {
  vector_t ahat = vec_unit(axis);
  double red_mass = reduced_mass(body1, body2);
  double u1 = velocity_along_axis(body1, ahat);
  double u2 = velocity_along_axis(body2, ahat);
  double impulse_mag = red_mass * (1 + elasticity) * (u2 - u1);

  vector_t impulse = vec_multiply(impulse_mag, ahat);
  return impulse;
}

void physics_collision_forcer(body_t *body1, body_t *body2, vector_t axis,
                              collision_event_aux_t *aux) {
  vector_t j1 = impulse_to_body_1(body1, body2, axis, aux->elasticity);
  vector_t j2 = vec_negate(j1);
  body_add_impulse(body1, j1);
  body_add_impulse(body2, j2);
}

void create_physics_collision(scene_t *scene, double elasticity, body_t *body1,
                              body_t *body2) {
  collision_event_aux_t *aux = malloc(sizeof(collision_event_aux_t));
  aux->elasticity = elasticity;
  create_collision(scene, body1, body2,
                   (collision_handler_t)physics_collision_forcer, aux, free);
}
