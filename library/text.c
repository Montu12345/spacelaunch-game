#include "body.h"
#include "forces.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

// need help with the freer

typedef struct text {
  char *words;
  vector_t text_position;
  double text_size;
  double numbers;
  vector_t text_dimensions;
  void *info;
} text_t;

enum text_type_t *text_type_init(enum text_type_t t) {
  enum text_type_t *text_type = malloc(sizeof(enum text_type_t));
  *text_type = t;
  return text_type;
}

text_t *text_words_init(char *words, vector_t text_position, double text_size,
                        vector_t text_dimensions) {
  text_t *params = malloc(sizeof(text_t));
  *params = (text_t){.words = words,
                     .text_position = text_position,
                     .text_size = text_size,
                     .numbers = 0,
                     .text_dimensions = text_dimensions,
                     .info = text_type_init(WORDS_ONLY)};
  return params;
}

text_t *text_words_erase_init(char *words, vector_t text_position, double text_size,
                        vector_t text_dimensions) {
  text_t *params = malloc(sizeof(text_t));
  *params = (text_t){.words = words,
                     .text_position = text_position,
                     .text_size = text_size,
                     .numbers = 0,
                     .text_dimensions = text_dimensions,
                     .info = text_type_init(WORDS_ONLY_ERASE)};
  return params;
}

text_t *text_numbers_init(char *words, vector_t text_position, double text_size,
                          double numbers, vector_t text_dimensions) {
  text_t *params = malloc(sizeof(text_t));
  *params = (text_t){.words = words,
                     .text_position = text_position,
                     .text_size = text_size,
                     .numbers = numbers,
                     .text_dimensions = text_dimensions,
                     .info = text_type_init(WORDS_NUMBER)};
  return params;
}

void text_set_words(text_t *text, char *words) { text->words = words; }

void text_set_text_position(text_t *text, vector_t text_position) {
  text->text_position = text_position;
}

void text_set_text_size(text_t *text, double text_size) {
  text->text_size = text_size;
}

void text_set_numbers(text_t *text, double numbers) { text->numbers = numbers; }

void text_set_text_dimensions(text_t *text, vector_t text_dimensions) {
  text->text_dimensions = text_dimensions;
}

char *text_get_words(text_t *text) { return text->words; }

vector_t text_get_text_position(text_t *text) { return text->text_position; }

double text_get_text_size(text_t *text) { return text->text_size; }

double text_get_numbers(text_t *text) { return text->numbers; }

vector_t text_get_text_dimensions(text_t *text) {
  return text->text_dimensions;
}

void *text_get_type(text_t *text) { return text->info; }
