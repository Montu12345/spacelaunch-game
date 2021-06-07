#include "body.h"
#include "forces.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

typedef struct text
{
  char *words;
  vector_t text_position;
  double text_size;
  double numbers;
  vector_t text_dimensions;
  void *info;
} text_t;

enum text_type_t *text_type_init(enum text_type_t t)
{
  enum text_type_t *text_type = malloc(sizeof(enum text_type_t));
  *text_type = t;
  return text_type;
}

text_t *text_init(char *words, vector_t text_position, double text_size,
                  double numbers, vector_t text_dimensions)
{
  text_t *params = malloc(sizeof(text_t));
  *params = (text_t){.words = words,
                     .text_position = text_position,
                     .text_size = text_size,
                     .numbers = numbers,
                     .text_dimensions = text_dimensions,
                     .info = text_type_init(WORDS_STAY)};
  return params;
}

void text_free(text_t *text)
{
  free(text->info);
  free(text);
}

void text_set_words(text_t *text, char *words) { text->words = words; }

void text_set_text_position(text_t *text, vector_t text_position)
{
  text->text_position = text_position;
}

void text_set_text_size(text_t *text, double text_size)
{
  text->text_size = text_size;
}

void text_set_numbers(text_t *text, double numbers) { text->numbers = numbers; }

void text_set_text_dimensions(text_t *text, vector_t text_dimensions)
{
  text->text_dimensions = text_dimensions;
}

void text_set_type(text_t *text, void *type)
{
  if (text->info)
  {
    free(text->info);
  }
  text->info = type;
}

char *text_get_words(text_t *text) { return text->words; }

vector_t text_get_text_position(text_t *text) { return text->text_position; }

double text_get_text_size(text_t *text) { return text->text_size; }

double text_get_numbers(text_t *text) { return text->numbers; }

vector_t text_get_text_dimensions(text_t *text)
{
  return text->text_dimensions;
}

void *text_get_type(text_t *text) { return text->info; }
