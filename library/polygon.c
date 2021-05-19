/** @file polygon.c
 *  @brief Implementations for a 2D polygon.
 *
 *  This contains the implementations for functionality to
 *  use a list of 2D vectors as a polygon.
 * 
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#include "polygon.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <float.h>
#include "vector.h"

const double CENTROID_AREA_SCALE_FACTOR = 1.0 / 6.0;
const double POLYGON_AREA_SCALE_FACTOR = 0.5;
const double POINT_INSIDE_TOLERANCE = 1e-7;
const double POINT_ON_TOLERANCE = 1e-4;

double polygon_area(list_t *polygon)
{
    double sum = 0.0;
    size_t n = list_size(polygon);
    for (size_t i = 0; i < n; i++)
    {
        size_t i_1 = (i + 1) % n;
        vector_t *a = (vector_t *)list_get(polygon, i);
        vector_t *b = (vector_t *)list_get(polygon, i_1);
        sum += vec_cross(*a, *b);
    }
    return POLYGON_AREA_SCALE_FACTOR * fabs(sum);
}

vector_t polygon_centroid(list_t *polygon)
{
    double c_x = 0.0;
    double c_y = 0.0;
    size_t n = list_size(polygon);
    for (size_t i = 0; i < n; i++)
    {
        size_t i_1 = (i + 1) % n;
        vector_t *v_i = (vector_t *)list_get(polygon, i);
        vector_t *v_i1 = (vector_t *)list_get(polygon, i_1);
        double det = vec_cross(*v_i, *v_i1);
        c_x += (v_i->x + v_i1->x) * det;
        c_y += (v_i->y + v_i1->y) * det;
    }
    double a_factor = CENTROID_AREA_SCALE_FACTOR / polygon_area(polygon);
    vector_t result = (vector_t){.x = c_x, .y = c_y};
    return vec_multiply(a_factor, result);
}

void polygon_translate(list_t *polygon, vector_t translation)
{
    for (size_t i = 0; i < list_size(polygon); i++)
    {
        vector_t *v = (vector_t *)list_get(polygon, i);
        *v = vec_add(*v, translation);
    }
}

void polygon_rotate(list_t *polygon, double angle, vector_t point)
{
    polygon_translate(polygon, vec_negate(point));
    for (size_t i = 0; i < list_size(polygon); i++)
    {
        vector_t *v = list_get(polygon, i);
        *v = vec_rotate(*v, angle);
    }
    polygon_translate(polygon, point);
}
