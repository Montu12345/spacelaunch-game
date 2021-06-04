/** @file sprite.c
 *  @brief Implementations for generating polygonal shapes.
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#include "sprite.h"
#include "vector.h"
#include <stdlib.h>

const size_t SPRITE_CIRCLE_POINTS_PER_RADIAN = 12;
const double SPRITE_PACMAN_MOUTH_ANGLE = M_PI / 3.0;

void sprite_add_radial_point(list_t *sprite, double radius, double angle) {
  vector_t new_side = (vector_t){.x = radius, .y = 0};
  vector_t rotated = vec_rotate(new_side, angle);
  vector_t *positioned_side = vec_malloc(rotated.x, rotated.y);
  list_add(sprite, positioned_side);
}

void sprite_add_arc(list_t *sprite, double radius, double start_angle,
                    double end_angle) {
  double angular_distance = end_angle - start_angle;
  size_t num_points =
      (size_t)SPRITE_CIRCLE_POINTS_PER_RADIAN * angular_distance;
  double current_angle = start_angle;
  double angular_step = angular_distance / num_points;
  for (size_t i = 0; i < num_points; i++) {
    sprite_add_radial_point(sprite, radius, current_angle);
    current_angle += angular_step;
  }
}

list_t *sprite_make_star(int num_points, double min_star_length,
                         double max_star_length) {
  list_t *star = list_init(num_points * 2, (free_func_t)vec_free);
  double vertex_length;
  double current_angle;
  for (int i = 0; i < 2 * num_points; i++) {
    vertex_length = min_star_length;
    if (i % 2 == 0) {
      vertex_length = max_star_length;
    }
    current_angle = M_PI * i / (1.0 * num_points);
    sprite_add_radial_point(star, vertex_length, current_angle);
  }
  return star;
}

list_t *sprite_make_invader(double radius, double bottom_depth) {
  list_t *invader = list_init(0, (free_func_t)vec_free);
  sprite_add_radial_point(invader, bottom_depth, 3 * M_PI / 2);
  sprite_add_arc(invader, radius, 0, M_PI);
  return invader;
}

list_t *sprite_make_ellipse(double x_axis_radius, double y_axis_radius) {
  size_t num_points = 1 + (size_t)(2 * M_PI * SPRITE_CIRCLE_POINTS_PER_RADIAN);
  list_t *ellipse = list_init(num_points, (free_func_t)vec_free);
  double angular_step = 2 * M_PI / num_points;
  for (double theta = 0; theta <= 2 * M_PI; theta += angular_step) {
    double x_part = pow(cos(theta) / x_axis_radius, 2);
    double y_part = pow(sin(theta) / y_axis_radius, 2);
    double r = pow(x_part + y_part, -0.5);
    sprite_add_radial_point(ellipse, r, theta);
  }
  return ellipse;
}

list_t *sprite_make_pacman(double radius) {
  double upper_lip_angle = SPRITE_PACMAN_MOUTH_ANGLE / 2.0;
  double bottom_lip_angle = 2 * M_PI - (SPRITE_PACMAN_MOUTH_ANGLE / 2.0);
  double total_pacman_arc = 2 * M_PI - SPRITE_PACMAN_MOUTH_ANGLE;
  size_t num_points =
      1 + (size_t)(total_pacman_arc * SPRITE_CIRCLE_POINTS_PER_RADIAN);
  list_t *pacman = list_init(num_points, (free_func_t)vec_free);
  sprite_add_radial_point(pacman, 0, 0);
  sprite_add_arc(pacman, radius, upper_lip_angle, bottom_lip_angle);
  return pacman;
}

list_t *sprite_make_circle(double radius) {
  size_t num_points = (size_t)(2 * M_PI * SPRITE_CIRCLE_POINTS_PER_RADIAN);
  list_t *circle = list_init(num_points, (free_func_t)vec_free);
  sprite_add_arc(circle, radius, 0, 2 * M_PI);
  return circle;
}

list_t *sprite_make_rect(double min_x, double max_x, double min_y,
                         double max_y) {
  list_t *rectangle = list_init(4, (free_func_t)vec_free);
  vector_t *bottom_left = vec_malloc(min_x, min_y);
  vector_t *bottom_right = vec_malloc(max_x, min_y);
  vector_t *top_left = vec_malloc(min_x, max_y);
  vector_t *top_right = vec_malloc(max_x, max_y);
  list_add(rectangle, top_right);
  list_add(rectangle, top_left);
  list_add(rectangle, bottom_left);
  list_add(rectangle, bottom_right);
  return rectangle;
}
