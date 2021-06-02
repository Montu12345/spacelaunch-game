#ifndef __SCENE_H__
#define __SCENE_H__

#include "body.h"
#include "list.h"
#include "text.h"

/**
 * A collection of bodies and force creators.
 * The scene automatically resizes to store
 * arbitrarily many bodies and force creators.
 */
typedef struct scene scene_t;

/**
 * Calculates the offset vector based off of a focal body and aux info.
 */
typedef vector_t (*camera_offset_func_t)(body_t *focal_body, void *aux);

/**
 * Dependent on the camera mode, calculates the amount to move a body to adjust 
 * for the desired scene change.
 * Offset is computed by the scene's camera_offset_func_t.
 */
typedef vector_t (*camera_mover_func_t)(vector_t offset, body_t *body);

/**
 * A function which adds some forces or impulses to bodies,
 * e.g. from collisions, gravity, or spring forces.
 * Takes in an auxiliary value that can store parameters or state.
 */
typedef void (*force_creator_t)(void *aux);

/**
 * Allocates memory for an empty scene.
 * Makes a reasonable guess of the number of bodies to allocate space for.
 * Asserts that the required memory is successfully allocated.
 *
 * @return the new scene
 */
scene_t *scene_init(void);

/**
 * Releases memory allocated for a given scene
 * and all the bodies and force creators it contains.
 *
 * @param scene a pointer to a scene returned from scene_init()
 */
void scene_free(scene_t *scene);

/**
 * Gets the number of bodies in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of bodies added with scene_add_body()
 */
size_t scene_bodies(scene_t *scene);

/**
 * Gets the number of forces in a given scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @return the number of forces added with scene_add_force_creator()
 */
size_t scene_forces(scene_t *scene);

/**
 * Gets the body at a given index in a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 * @return a pointer to the body at the given index
 */
body_t *scene_get_body(scene_t *scene, size_t index);

/**
 * Adds a body to a scene.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param body a pointer to the body to add to the scene
 */
void scene_add_body(scene_t *scene, body_t *body);

/**
 * @deprecated Use body_remove() instead
 *
 * Removes and frees the body at a given index from a scene.
 * Asserts that the index is valid.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param index the index of the body in the scene (starting at 0)
 */
void scene_remove_body(scene_t *scene, size_t index);

/**
 * @deprecated Use scene_add_bodies_force_creator() instead
 * so the scene knows which bodies the force creator depends on
 */
void scene_add_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    free_func_t freer);

/**
 * Adds a force creator to a scene,
 * to be invoked every time scene_tick() is called.
 * The auxiliary value is passed to the force creator each time it is called.
 * The force creator is registered with a list of bodies it applies to,
 * so it can be removed when any one of the bodies is removed.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param forcer a force creator function
 * @param aux an auxiliary value to pass to forcer when it is called
 * @param bodies the list of bodies affected by the force creator.
 *   The force creator will be removed if any of these bodies are removed.
 *   This list does not own the bodies, so its freer should be NULL.
 * @param freer if non-NULL, a function to call in order to free aux
 */
void scene_add_bodies_force_creator(
    scene_t *scene,
    force_creator_t forcer,
    void *aux,
    list_t *bodies,
    free_func_t freer);

/**
 * Adds a force camera management system to the scene. The camera offset function calculates some
 * vector to base the camera mover funciton on. The camera mover applies that vector dependent on the 
 * body's camera mode. 
 * 
 * Note:
 * camera_offset and camera_mover run every scene tick. 
 * Does not run if there is not a focal element.
 * 
 * @param scene a pointer to a scene returned from scene_init()
 * @param camera_offset function to calculate the offset to input to the camera mover
 * @param camera_mover function to calculate the movement of a given body for the camera
 * @param camera_aux additional info for the offset function
 */
void scene_add_camera_management(
    scene_t *scene,
    camera_offset_func_t camera_offset,
    camera_mover_func_t camera_mover,
    void *camera_aux, 
    free_func_t camera_aux_free);

/**
 * Sets the focal body for the camera management system.
 * 
 * The focal body is the body the camera_offset is most concerned with,
 * which is likely the game character.
 * 
 * Useful for the purpose of resetting scenes. Once the focal body is freed, be sure
 * to set the focal body to an active body_t*.
 * 
 * @param scene a pointer to a scene returned from scene_init()
 * @param focal_body function to calculate the offset to input to the camera mover
 */
void scene_set_focal_body(scene_t *scene, body_t *focal_body);

/**
 * Executes a tick of a given scene over a small time interval.
 * This requires executing all the force creators
 * and then ticking each body (see body_tick()).
 * If any bodies are marked for removal, they should be removed from the scene
 * and freed, along with any force creators acting on them.
 *
 * @param scene a pointer to a scene returned from scene_init()
 * @param dt the time elapsed since the last tick, in seconds
 */
void scene_tick(scene_t *scene, double dt);

void scene_add_text(scene_t *scene, text_t *text);

size_t scene_text(scene_t *scene);

text_t *scene_get_text(scene_t *scene, size_t index);

#endif // #ifndef __SCENE_H__
