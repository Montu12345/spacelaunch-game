/** @file scene.c
 *  @brief Implementations for a physics engine to manage bodies
 *
 *  @author Alexis Wang, Ian Fowler, Shevali Kadakia, Ezra Johnson
 *  @bug Objects may fall through the ground at high speeds
 *  @todo Generalize collisions, allow for more than one collision flag.
 */
#include "scene.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include "text.h"

const size_t BODIES_DEFAULT_CAPACITY = 32;
const size_t TEXT_DEFAULT_CAPACITY = 32;
const size_t FORCES_DEFAULT_CAPACITY = 64;
const size_t INVALID_FOCAL_IDX = -1;

typedef struct force
{
    force_creator_t forcer;
    void *aux;
    free_func_t aux_freer;
    list_t *bodies;
} force_t;

force_t *force_init(force_creator_t forcer,
                    void *aux,
                    free_func_t aux_freer,
                    list_t *bodies)
{
    force_t *force = malloc(sizeof(force_t));
    force->forcer = forcer;
    force->aux = aux;
    force->aux_freer = aux_freer;
    force->bodies = bodies;
    return force;
}

void force_free(force_t *force)
{
    if (force->aux_freer != NULL)
    {
        force->aux_freer(force->aux);
    }
    if (force->bodies != NULL)
    {
        list_free(force->bodies);
    }
    free(force);
}

typedef struct scene
{
    list_t *bodies;
    list_t *forces;
    camera_offset_func_t camera_offset;
    camera_mover_func_t camera_mover;
    body_t *focal_body;
    void *camera_aux;
    free_func_t camera_aux_free;
    list_t *text;
} scene_t;

scene_t *scene_init()
{
    list_t *bodies = list_init(BODIES_DEFAULT_CAPACITY, (free_func_t)body_free);
    list_t *forces = list_init(FORCES_DEFAULT_CAPACITY, (free_func_t)force_free);
    list_t *text = list_init(TEXT_DEFAULT_CAPACITY, (free_func_t)free);
    scene_t *scene = malloc(sizeof(scene_t));
    assert(scene != NULL);
    scene->bodies = bodies;
    scene->forces = forces;
    scene->text = text;
    scene->camera_aux = NULL;
    scene->focal_body = NULL;
    return scene;
}

void scene_free(scene_t *scene)
{
    list_free(scene->bodies);
    list_free(scene->forces);
    list_free(scene->text);
    if (scene->camera_aux_free)
    {
        ((free_func_t)scene->camera_aux_free)(scene->camera_aux);
    }
    free(scene);
}

size_t scene_bodies(scene_t *scene)
{
    return list_size(scene->bodies);
}

size_t scene_forces(scene_t *scene)
{
    return list_size(scene->forces);
}

body_t *scene_get_body(scene_t *scene, size_t index)
{
    assert(index < list_size(scene->bodies));
    assert(index >= 0);
    return list_get(scene->bodies, index);
}

void scene_add_body(scene_t *scene, body_t *body)
{
    list_add(scene->bodies, body);
}

void scene_add_force_creator(scene_t *scene, force_creator_t forcer, void *aux, free_func_t freer)
{
    scene_add_bodies_force_creator(scene, forcer, aux, NULL, freer);
}

void scene_add_bodies_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    list_t *bodies,
    free_func_t freer)
{
    force_t *force = force_init(forcer, aux, freer, bodies);
    list_add(scene->forces, force);
}

void scene_remove_body(scene_t *scene, size_t index)
{
    assert(index >= 0 && index < scene_bodies(scene));
    body_remove(scene_get_body(scene, index));
}

bool force_is_removed(force_t *force)
{
    for (size_t idx = 0; idx < list_size(force->bodies); idx++)
    {
        body_t *curr_body = list_get(force->bodies, idx);
        if (body_is_removed(curr_body))
        {
            return true;
        }
    }
    return false;
}

bool idx_is_valid(size_t idx, list_t *list)
{
    return idx >= 0 && idx < list_size(list);
}

void scene_add_camera_management(
    scene_t *scene,
    camera_offset_func_t camera_offset,
    camera_mover_func_t camera_mover,
    void *camera_aux,
    free_func_t camera_aux_free)
{
    scene->camera_aux = camera_aux;
    scene->camera_offset = camera_offset;
    scene->camera_mover = camera_mover;
    scene->camera_aux_free = camera_aux_free;
}

void scene_set_focal_body(scene_t *scene, body_t *focal_body)
{
    scene->focal_body = focal_body;
}

void apply_camera(scene_t *scene)
{
    body_t *focal_body = scene->focal_body;
    if (!focal_body)
    {
        return;
    }
    vector_t offset = scene->camera_offset(focal_body, scene->camera_aux);
    for (size_t idx = 0; idx < scene_bodies(scene); idx++)
    {
        body_t *current_body = scene_get_body(scene, idx);
        vector_t movement = scene->camera_mover(offset, current_body);
        body_adjust_for_camera(current_body, movement);
    }
}

void apply_forces(scene_t *scene)
{
    for (size_t idx = 0; idx < scene_forces(scene); idx++)
    {
        force_t *force = list_get(scene->forces, idx);
        force->forcer(force->aux);
    }
}

void clean_forces(scene_t *scene)
{
    for (size_t idx = 0; idx < scene_forces(scene); idx++)
    {
        force_t *force = list_get(scene->forces, idx);
        if (force->bodies != NULL && force_is_removed(force))
        {
            list_remove(scene->forces, idx);
            force_free(force);
            idx--;
        }
    }
}

void move_and_clean_bodies(scene_t *scene, double dt)
{
    for (size_t idx = 0; idx < scene_bodies(scene); idx++)
    {
        body_t *current_body = scene_get_body(scene, idx);
        body_tick(current_body, dt);
        if (body_is_removed(current_body))
        {
            list_remove(scene->bodies, idx);
            body_free(current_body);
            idx--;
        }
    }
}

void scene_add_text(scene_t *scene, text_t *text)
{
    list_add(scene->text, text);
}

size_t scene_text(scene_t *scene)
{
    return list_size(scene->text);
}

text_t *scene_get_text(scene_t *scene, size_t index)
{
    text_t *text = list_get(scene->text, index);
    return text;
}

void scene_tick(scene_t *scene, double dt)
{
    apply_forces(scene);
    clean_forces(scene);
    move_and_clean_bodies(scene, dt);
    apply_camera(scene);
}
