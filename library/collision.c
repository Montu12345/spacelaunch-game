#include "collision.h"

const double AXIS_ROTATION_FROM_SIDE = M_PI / 2;

typedef struct range
{
  double min;
  double max;
} range_t;

typedef struct collis_deriv_info
{
  collision_info_t info;
  double overlap;
} collis_deriv_info_t;

bool point_within_range(double point, range_t range)
{
  return range.min <= point && point <= range.max;
}

double range_mag(range_t a)
{
  return a.max - a.min;
}

bool range_a_intersects_b(range_t range_a, range_t range_b)
{
  bool mincheck = point_within_range(range_a.min, range_b);
  bool maxcheck = point_within_range(range_a.max, range_b);
  return mincheck || maxcheck;
}

bool ranges_intersect(range_t range_a, range_t range_b)
{
  bool acheck = range_a_intersects_b(range_a, range_b);
  bool bcheck = range_a_intersects_b(range_b, range_a);
  return acheck || bcheck;
}

double range_overlap(range_t a, range_t b)
{
  if (!ranges_intersect(a, b))
  {
    return 0;
  }
  return fabs(fmin(a.max, b.max) - fmax(a.min, b.min));
}

double signed_projection_magnitude(vector_t axis, vector_t vertex)
{
  vector_t proj = vec_project(axis, vertex);
  double proj_mag = vec_magnitude(proj);
  // Get unit vectors of projection and axis
  vector_t phat = vec_unit(proj);
  vector_t ahat = vec_unit(axis);
  vector_t diff = vec_subtract(phat, ahat);

  // If unit vectors are the same, then they are in the same direction
  double diff_mag = vec_magnitude(diff);

  // If the difference of unit vectors is closer to 2 than zero, "negative" magnitude.
  double dist_to_zero = fabs(diff_mag);
  double dist_to_two = fabs(diff_mag - 2);
  if (dist_to_two < dist_to_zero)
  {
    proj_mag *= -1;
  }
  return proj_mag;
}

range_t axis_projection_range(list_t *polygon, vector_t axis)
{
  double min;
  double max;

  size_t n = list_size(polygon);
  for (size_t i = 0; i < n; i++)
  {
    vector_t *vertex = (vector_t *)list_get(polygon, i);
    double proj_mag = signed_projection_magnitude(axis, *vertex);
    if (i == 0)
    {
      min = proj_mag;
      max = proj_mag;
    }
    else
    {
      min = fmin(proj_mag, min);
      max = fmax(proj_mag, max);
    }
  }

  return (range_t){
      .min = min,
      .max = max,
  };
}

collis_deriv_info_t side_overlap(list_t *shape_a, list_t *shape_b, size_t idx1, size_t idx2)
{
  vector_t *a = (vector_t *)list_get(shape_a, idx1);
  vector_t *b = (vector_t *)list_get(shape_a, idx2);
  vector_t side = vec_subtract(*a, *b);
  vector_t axis = vec_rotate(side, AXIS_ROTATION_FROM_SIDE);
  range_t range_a = axis_projection_range(shape_a, axis);
  range_t range_b = axis_projection_range(shape_b, axis);
  double overlap = range_overlap(range_a, range_b);
  collision_info_t info = (collision_info_t){
      .axis = axis,
      .collided = ranges_intersect(range_a, range_b),
  };
  return (collis_deriv_info_t){
      .info = info,
      .overlap = overlap};
}

collis_deriv_info_t best_collision_axis(collis_deriv_info_t a, collis_deriv_info_t b)
{
  return a.overlap < b.overlap ? a : b;
}

collis_deriv_info_t axes_match_shape_a(list_t *shape_a, list_t *shape_b)
{
  size_t n = list_size(shape_a);
  collis_deriv_info_t best;

  for (size_t idx1 = 0; idx1 < n; idx1++)
  {
    size_t idx2 = (idx1 + 1) % n;
    collis_deriv_info_t col = side_overlap(shape_a, shape_b, idx1, idx2);
    if (!col.info.collided)
    {
      return col;
    }
    best = idx1 == 0 ? col : best_collision_axis(best, col);
  }
  return best;
}

collision_info_t find_collision(list_t *shape1, list_t *shape2)
{
  collis_deriv_info_t respect1 = axes_match_shape_a(shape1, shape2);
  collis_deriv_info_t respect2 = axes_match_shape_a(shape2, shape1);
  if (!(respect1.info.collided && respect2.info.collided))
  {
    return (collision_info_t){
        .axis = VEC_ZERO,
        .collided = false};
  }
  return best_collision_axis(respect1, respect2).info;
  // return (collision_info_t){
  //     .axis = vec_subtract(polygon_centroid(shape2), polygon_centroid(shape1)),
  //     .collided = true};
  // return (collision_info_t){
  //     .axis = vec_rotate(best_collision_axis(respect1, respect2).info.axis, AXIS_ROTATION_FROM_SIDE),
  //     .collided = true};
}
