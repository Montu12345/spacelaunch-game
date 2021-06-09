#ifndef __TEXT_H__
#define __TEXT_H__

#include "body.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

typedef struct text text_t;

// types of text
enum text_type_t {
  WORDS_STAY,
  WORDS_ERASE,
  WORDS_POWERUP,
};

/**
 * Frees the text.
 *
 * @param text text
 */
void text_free(text_t *text);

/**
 * Creates an text-type enum
 *
 * @param t enum type
 * @return text-type enum
 */
enum text_type_t *text_type_init(enum text_type_t t);

/**
 * Creates the text
 *
 * @param words words in the text
 * @param text_position position of text
 * @param text_size size of text
 * @param numbers numbers to be entered at the end of the sentence
 * @param text_dimensions dimentions of text
 */
text_t *text_init(char *words, vector_t text_position, double text_size,
                          double numbers, vector_t text_dimensions);

/**
 * Sets the words to be in the text
 *
 * @param text text object
 * @param endzone string
 */
void text_set_words(text_t *text, char *words);

/**
 * Sets the position of the text
 *
 * @param text text object
 * @param test_position text position
 */
void text_set_text_position(text_t *text, vector_t text_position);

/**
 * Sets the size of the text
 *
 * @param text text object
 * @param test_size text size
 */
void text_set_text_size(text_t *text, double text_size);

/**
 * Sets the numbers in the text
 *
 * @param text text object
 * @param numbers numbers to add into text
 */
void text_set_numbers(text_t *text, double numbers);

/**
 * Sets the dimensions of the text
 *
 * @param text text object
 * @param test_position text dimensions
 */
void text_set_text_dimensions(text_t *text, vector_t text_dimensions);

/**
 * Sets the type of the text
 *
 * @param text text object
 * @param type text type
 */
void text_set_type(text_t *text, void* type);

/**
 * Gets the words of the text
 *
 * @param text text object
 * @return string of the text
 */
char *text_get_words(text_t *text);

/**
 * Gets the position of the text
 *
 * @param text text object
 * @return position of the text
 */
vector_t text_get_text_position(text_t *text);

/**
 * Gets the size of the text
 *
 * @param text text object
 * @return size of the text
 */
double text_get_text_size(text_t *text);

/**
 * Gets the numbers of the text
 *
 * @param text text object
 * @return ints of the text
 */
double text_get_numbers(text_t *text);

/**
 * Gets the dimensions of the text
 *
 * @param text text object
 * @return dimensions of the text
 */
vector_t text_get_text_dimensions(text_t *text);

/**
 * Gets the type of the text
 *
 * @param text text object
 * @return type of the text
 */
void *text_get_type(text_t *text);

#endif // #ifndef __TEXT_H__