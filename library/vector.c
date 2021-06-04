/** @file vector.c
 *  @brief Function implementations for a 2D vector.
 *
 *  This contains the implementations for
 *  some 2D vector functionality.
 *
 *  @author Alexis Wang, Ian Fowler
 *  @bug No known bugs.
 */

#include "vector.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const vector_t VEC_ZERO = {.x = 0, .y = 0};

vector_t *vec_malloc(double x, double y) {
  vector_t *vector = malloc(sizeof(vector_t));
  vector->x = x;
  vector->y = y;
  return vector;
}

void vec_free(vector_t *vector) { free(vector); }

vector_t vec_add(vector_t v1, vector_t v2) {
  vector_t sum = {.x = v1.x + v2.x, .y = v1.y + v2.y};
  return sum;
}

vector_t vec_negate(vector_t v) { return vec_multiply(-1.0, v); }

double vec_magnitude(vector_t v) { return sqrt(vec_dot(v, v)); }

vector_t vec_subtract(vector_t v1, vector_t v2) {
  vector_t negated_vector = vec_negate(v2);
  return vec_add(v1, negated_vector);
}

vector_t vec_multiply(double scalar, vector_t v) {
  vector_t new_vector = {.x = v.x * scalar, .y = v.y * scalar};
  return new_vector;
}

double vec_dot(vector_t v1, vector_t v2) { return v1.x * v2.x + v1.y * v2.y; }

double vec_cross(vector_t v1, vector_t v2) { return v1.x * v2.y - v1.y * v2.x; }

vector_t vec_rotate(vector_t v, double angle) {
  double x_component = cos(angle) * v.x - v.y * sin(angle);
  double y_component = sin(angle) * v.x + v.y * cos(angle);
  vector_t rotated_vector = {.x = x_component, .y = y_component};
  return rotated_vector;
}

vector_t vec_project(vector_t v1, vector_t v2) {
  double scale = vec_dot(v2, v1) / vec_dot(v1, v1);
  return vec_multiply(scale, v1);
}

vector_t vec_project_to_line(vector_t p, vector_t l1, vector_t l2) {
  vector_t a = vec_subtract(p, l1);
  vector_t b = vec_subtract(l2, l1);
  vector_t proj = vec_project(b, a);
  return proj;
}

double vec_angle_between(vector_t v1, vector_t v2) {
  double cos_ang = vec_dot(v1, v2) / (vec_magnitude(v1) * vec_magnitude(v2));
  double ang = acos(cos_ang);
  return ang;
}

vector_t vec_unit(vector_t v) {
  double magnitude = vec_magnitude(v);
  vector_t unit = vec_multiply(1.0 / magnitude, v);
  return unit;
}

vector_t vec_reflect_across_line(vector_t v, vector_t line) {
  vector_t proj = vec_project(line, v);
  vector_t reflection = vec_subtract(v, vec_multiply(2, proj));
  return reflection;
}
