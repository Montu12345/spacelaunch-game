#ifndef __TEXT_H__
#define __TEXT_H__

#include "body.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

// need help with the freer

typedef struct text text_t;

enum text_type_t {
  WORDS_ONLY,
  WORDS_NUMBER,
  WORDS_ONLY_ERASE,
};

text_t *text_words_init(char *words, vector_t text_position, double text_size,
                        vector_t text_dimensions);

text_t *text_numbers_init(char *words, vector_t text_position, double text_size,
                          double numbers, vector_t text_dimensions);

text_t *text_words_erase_init(char *words, vector_t text_position, double text_size,
                        vector_t text_dimensions);

void text_set_words(text_t *text, char *words);

void text_set_text_position(text_t *text, vector_t text_position);

void text_set_text_size(text_t *text, double text_size);

void text_set_numbers(text_t *text, double numbers);

void text_set_text_dimensions(text_t *text, vector_t text_dimensions);

char *text_get_words(text_t *text);

vector_t text_get_text_position(text_t *text);

double text_get_text_size(text_t *text);

double text_get_numbers(text_t *text);

vector_t text_get_text_dimensions(text_t *text);

void *text_get_type(text_t *text);

#endif // #ifndef __TEXT_H__