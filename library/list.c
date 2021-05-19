/** @file list.c
 *  @brief Function implementations for a list of 2D vectors.
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug No known bugs.
 */

#include "list.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>

typedef struct list
{
    size_t size;
    size_t capacity;
    void **data;
    free_func_t freer;
} list_t;

const float RESIZE_SCALE_FACTOR = 2;

list_t *list_init(size_t initial_size, free_func_t freer)
{
    list_t *list = malloc(sizeof(list_t));
    assert(list != NULL);
    list->size = 0;
    list->capacity = initial_size;
    list->data = malloc(sizeof(list_t *) * initial_size);
    assert(list->data != NULL);
    list->freer = freer;
    return list;
}

void list_free(list_t *list)
{
    if (list->freer != NULL)
    {
        for (size_t i = 0; i < list->size; i++)
        {
            list->freer(list->data[i]);
        }
    }
    free(list->data);
    free(list);
}

size_t list_size(list_t *list)
{
    return list->size;
}

list_t *list_resize(list_t *list)
{
    if (list->size < list->capacity)
    {
        return list;
    }
    size_t new_size = (size_t)RESIZE_SCALE_FACTOR * list->capacity;
    if (new_size == 0)
    {
        new_size = 2;
    }
    list->data = realloc(list->data, new_size * sizeof(list_t *));
    list->capacity = new_size;
    return list;
}

void *list_get(list_t *list, size_t index)
{
    assert(index >= 0);
    assert(index < list->size);
    void *vec = list->data[index];
    return vec;
}

void *list_remove(list_t *list, size_t index)
{
    assert(index >= 0);
    assert(index < list_size(list));
    void *result = list->data[index];
    for (size_t idx = index; idx < list_size(list) - 1; idx++)
    {
        list->data[idx] = list_get(list, idx + 1);
    }
    list->size--;
    return result;
}

void list_add(list_t *list, void *value)
{
    assert(value != NULL);
    size_t size_list = list->size;
    list = list_resize(list);
    list->data[size_list] = value;
    list->size++;
}
