/** @file color.c
 *  @brief Implementations for functions working with colors stored in rgb.
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#include "color.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

rgb_color_t rgb_init(float red, float green, float blue) {
  rgb_color_t color = {.r = red, .g = green, .b = blue};
  return color;
}

rgb_color_t rgb_init_random_bright() {
  return rgb_from_hsv_init(rand() % 361, 1, 1);
}

rgb_color_t rgb_from_hsv_init(float hue, float saturation, float value) {
  float c = value * saturation;
  float x = c * (1 - fabs(fmod((hue / 60.0), 2) - 1));
  float m = value - c;
  float r_prime;
  float g_prime;
  float b_prime;
  if (hue < 60) {
    r_prime = c;
    g_prime = x;
    b_prime = 0;
  } else if (hue < 120) {
    r_prime = x;
    g_prime = c;
    b_prime = 0;
  } else if (hue < 180) {
    r_prime = 0;
    g_prime = c;
    b_prime = x;
  } else if (hue < 240) {
    r_prime = 0;
    g_prime = x;
    b_prime = c;
  } else if (hue < 300) {
    r_prime = x;
    g_prime = 0;
    b_prime = c;
  } else {
    r_prime = c;
    g_prime = 0;
    b_prime = x;
  }

  return rgb_init((r_prime + m), (g_prime + m), (b_prime + m));
}
