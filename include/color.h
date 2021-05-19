/** @file rgb_color.h
 *  @brief Function prototypes for working with colors stored in rgb.
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#ifndef __COLOR_H__
#define __COLOR_H__

/**
 * A color represented by parts red (r), green (g), and blue (b).
 * The value of 1 for a particular color corresponds to displaying the color 
 * in full brighness, and 0 represents not displaying the color at all. 
 */
typedef struct rgb_color
{
    float r;
    float g;
    float b;
} rgb_color_t;

/** @brief Initializes an rgb_color_t with the specified parts red, green, and blue.
 *
 *  Assert colors are within range. 
 * 
 *  @param red the portion of red in the color, from 0 (none) to 1 (max)
 *  @param green the portion of green in the color, from 0 (none) to 1 (max)
 *  @param blue the portion of blue in the color, from 0 (none) to 1 (max)
 *  @return an rgb_color containing red, green, blue.
 */
rgb_color_t rgb_init(float red, float green, float blue);

/** @brief Initializes an random bright color.
 *
 *  Use HSV with hue/saturation at 1, vary the hue.
 *  
 *  @return an rgb_color containing red, green, blue.
 */
rgb_color_t rgb_init_random_bright();

/** @brief Initializes an rgb_color_t with the specified parts red, green, and blue.
 *
 *  Follow formula https://www.rapidtables.com/convert/color/hsv-to-rgb.html
 *  Assert values are within respective ranges
 * 
 *  @param hue angle along the color wheel, from 0 to 360 degrees
 *  @param saturation the vibrancy of the color, from 0 (none) to 1 (max)
 *  @param value the brightness of the color, from 0 (none) to 1 (max)
 *  @return an rgb_color containing red, green, blue.
 */
rgb_color_t rgb_from_hsv_init(float hue, float saturation, float value);

#endif // #ifndef __COLOR_H__
