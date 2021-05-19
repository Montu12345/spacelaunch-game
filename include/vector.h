/** @file vector.h
 *  @brief Function prototypes for 2D vector operations. 
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#ifndef __VECTOR_H__
#define __VECTOR_H__

/**
 * A real-valued 2-dimensional vector.
 * Positive x is towards the right; positive y is towards the top.
 * vector_t is defined here instead of vector.c because it is passed *by value*.
 */
typedef struct vector
{
    double x;
    double y;
} vector_t;

/**
 * The zero vector, i.e. (0, 0).
 * "extern" declares this global variable without allocating memory for it.
 * You will need to define "const vector_t VEC_ZERO = ..." in vector.c.
 */
extern const vector_t VEC_ZERO;

/**
 * Adds two vectors.
 * Performs the usual componentwise vector sum.
 *
 * @param v1 the first vector
 * @param v2 the second vector
 * @return v1 + v2
 */
vector_t *vec_malloc(double x, double y);

/** @brief Frees the value of a vector_t
 * 
 *  @param vector the vector to free
 *  @return none
 */
void vec_free(vector_t *vector);

/**
 * Adds two vectors.
 * Performs the usual componentwise vector sum.
 *
 * @param v1 the first vector
 * @param v2 the second vector
 * @return v1 + v2
 */
vector_t vec_add(vector_t v1, vector_t v2);

/**
 * Subtracts two vectors.
 * Performs the usual componentwise vector difference.
 *
 * @param v1 the first vector
 * @param v2 the second vector
 * @return v1 - v2
 */
vector_t vec_subtract(vector_t v1, vector_t v2);

/**
 * Computes the additive inverse of a vector.
 * This is equivalent to multiplying by -1.
 *
 * @param v the vector whose inverse to compute
 * @return -v
 */
vector_t vec_negate(vector_t v);

/**
 * Computes the magnitude of a vector
 *
 * @param v the vector whose magnitude to compute
 * @return -double
 */
double vec_magnitude(vector_t v);

/**
 * Multiplies a vector by a scalar.
 * Performs the usual componentwise product.
 *
 * @param scalar the number to multiply the vector by
 * @param v the vector to scale
 * @return scalar * v
 */
vector_t vec_multiply(double scalar, vector_t v);

/**
 * Computes the dot product of two vectors.
 * See https://en.wikipedia.org/wiki/Dot_product#Algebraic_definition.
 *
 * @param v1 the first vector
 * @param v2 the second vector
 * @return v1 . v2
 */
double vec_dot(vector_t v1, vector_t v2);

/**
 * Computes the cross product of two vectors,
 * which lies along the z-axis.
 * See https://en.wikipedia.org/wiki/Cross_product#Computing_the_cross_product.
 *
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the z-component of v1 x v2
 */
double vec_cross(vector_t v1, vector_t v2);

/**
 * Rotates a vector by an angle around (0, 0).
 * The angle is given in radians.
 * Positive angles are counterclockwise, according to the right hand rule.
 * See https://en.wikipedia.org/wiki/Rotation_matrix.
 * (You can derive this matrix by noticing that rotation by a fixed angle
 * is linear and then computing what it does to (1, 0) and (0, 1).)
 *
 * @param v the vector to rotate
 * @param angle the angle to rotate the vector
 * @return v rotated by the given angle
 */
vector_t vec_rotate(vector_t v, double angle);

/**
 * Computes the angle between two given vectors.
 *
 * @param v1 the first vector
 * @param v2 the second vector
 * @return the angle between the vectors, in radians. 
 */
double vec_angle_between(vector_t v1, vector_t v2);

/**
 * Reflects a vector v across a line. 
 *
 * @param v the vector to reflect across the line
 * @param line the line (vector_t) to reflect v across
 * @return the reflected vector
 */
vector_t vec_reflect_across_line(vector_t v, vector_t line);

/**
 * Projects vector v2 onto vector v1
 *
 * @param v1 the vector to be projected onto
 * @param v2 the vector to project onto v1 
 * @return the projection vector 
 */
vector_t vec_project(vector_t v1, vector_t v2);

/**
 * Returns a unit vector in the direction of v
 *
 * @param v the vector for which to return the unit vector
 * @return the vector of magnitude 1 in the direction of v
 */
vector_t vec_unit(vector_t v);

/**
 * Projects point p onto a line composed of points l1 and l2.
 * The magnitude of this projection is the distance between point p
 * and the line between l1 and l2.
 *
 * @param p the point
 * @param l1 the first point on the line
 * @param l2 the second point on the line
 * @return the projection vector from p to the line between l1 and l2
 */
vector_t vec_project_to_line(vector_t p, vector_t l1, vector_t l2);

#endif // #ifndef __VECTOR_H__
