#include "forces.h"
#include "body.h"
#include "scene.h"
#include <math.h>
#include <stdio.h>

// need help with the freer

typedef struct text
{
    char *words;
    vector_t text_position;
    double text_size;
    double numbers;
    vector_t text_dimensions;
} text_t;

text_t *text_init(
    char *words, 
    vector_t text_position, 
    double text_size,
    double numbers,
    vector_t text_dimensions)
{
    text_t *params = malloc(sizeof(text_t));
    *params = (text_t){
        .words = words,
        .text_position = text_position,
        .text_size = text_size,
        .numbers = numbers,
        .text_dimensions = text_dimensions};
    return params;
}

void text_set_words(text_t *text, char *words){
    text->words = words;
}

void text_set_text_position(text_t *text, vector_t text_position){
    text->text_position = text_position;
}

void text_set_text_size(text_t *text, double text_size){
    text->text_size = text_size;
}

void text_set_numbers(text_t *text, double numbers){
    text->numbers = numbers;
}

void text_set_text_dimensions(text_t *text, vector_t text_dimensions){
    text->text_dimensions = text_dimensions;
}

char *text_get_words(text_t *text){
    return text->words;
}

vector_t text_get_text_position(text_t *text){
    return text->text_position;
}

double text_get_text_size(text_t *text){
    return text->text_size;
}

double text_get_numbers(text_t *text){
    return text->numbers;
}

vector_t text_get_text_dimensions(text_t *text){
    return text->text_dimensions;
}