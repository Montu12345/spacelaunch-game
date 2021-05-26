#include "body.h"
#include <stdio.h>
#include <assert.h>

const double BODY_DEFAULT_ANGULAR_VELOCITY = 0.0;
const double BODY_DEFAULT_ANGULAR_POSITION = 0.0;
const double BODY_DEFAULT_BOUNCINESS = 1.0;
const double BODY_DEFAULT_VELOCITY_X = 0.0;
const double BODY_DEFAULT_VELOCITY_Y = 0.0;
const bool BODY_IS_MOVABLE = true;

typedef struct body_appearance
{
    list_t *shape;
    rgb_color_t color;
} body_appearance_t;

typedef struct body_physical_properties
{
    double mass;
    double bounciness;
    bool movable;
} body_physical_properties_t;

typedef struct body_kinematic_variables
{
    vector_t velocity;
    vector_t position;
    double angular_velocity;
    double angular_position;
    vector_t net_force;
    vector_t impulse;
} body_kinematic_variables_t;

typedef struct body_aux_properties
{
    void *info;
    free_func_t info_freer;
    bool is_removed;
    camera_mode_t camera_mode;
    vector_t camera_movement;
} body_aux_properties_t;

typedef struct body
{
    body_kinematic_variables_t kinematic_variables;
    body_physical_properties_t physical_properties;
    body_appearance_t appearance;
    body_aux_properties_t aux;
} body_t;

body_appearance_t body_appearance_init(
    list_t *shape,
    rgb_color_t color)
{
    body_appearance_t appearance = {
        .shape = shape,
        .color = color};
    return appearance;
};

body_physical_properties_t body_physical_properties_init(
    double mass,
    double bounciness,
    bool movable)
{
    body_physical_properties_t physical_properties = {
        .mass = mass,
        .bounciness = bounciness,
        .movable = movable};
    return physical_properties;
};

body_aux_properties_t body_aux_properties_init(
    void *info,
    free_func_t info_freer,
    bool is_removed,
    camera_mode_t camera_mode,
    vector_t camera_movement)
{
    body_aux_properties_t aux = {
        .info = info,
        .info_freer = info_freer,
        .is_removed = is_removed,
        .camera_mode = camera_mode,
        .camera_movement = camera_movement};
    return aux;
};

body_kinematic_variables_t body_kinematic_variables_init(
    vector_t velocity,
    vector_t position,
    double angular_velocity,
    double angular_position,
    vector_t net_force,
    vector_t impulse)
{
    body_kinematic_variables_t kinematic_variables = {
        .velocity = velocity,
        .position = position,
        .angular_velocity = angular_velocity,
        .angular_position = angular_position,
        .net_force = net_force,
        .impulse = impulse};

    return kinematic_variables;
};

body_t *body_init(list_t *shape, double mass, rgb_color_t color)
{
    vector_t position = polygon_centroid(shape);
    body_t *body = malloc(sizeof(body_t));
    body_kinematic_variables_t kinematic = body_kinematic_variables_init(
        (vector_t){.x = BODY_DEFAULT_VELOCITY_X, .y = BODY_DEFAULT_VELOCITY_Y},
        (vector_t){.x = position.x, .y = position.y},
        BODY_DEFAULT_ANGULAR_VELOCITY,
        BODY_DEFAULT_ANGULAR_POSITION,
        VEC_ZERO,
        VEC_ZERO);
    body_physical_properties_t physical = body_physical_properties_init(
        mass,
        BODY_DEFAULT_BOUNCINESS,
        BODY_IS_MOVABLE);
    rgb_color_t color_pointer = rgb_init(color.r, color.g, color.b);
    body_appearance_t appearance = body_appearance_init(shape, color_pointer);
    body_aux_properties_t aux = body_aux_properties_init(NULL, NULL, false, LOCKED, VEC_ZERO);
    *body = (body_t){
        .kinematic_variables = kinematic,
        .physical_properties = physical,
        .appearance = appearance,
        .aux = aux};
    return body;
};

body_t *body_init_with_info(
    list_t *shape,
    double mass,
    rgb_color_t color,
    void *info,
    free_func_t info_freer)
{
    body_t *body = body_init(shape, mass, color);
    body->aux.info = info;
    body->aux.info_freer = info_freer;
    return body;
}

void *body_get_info(body_t *body)
{
    return body->aux.info;
}

void body_free(body_t *body)
{
    list_free(body->appearance.shape);
    if (body->aux.info_freer != NULL)
    {
        body->aux.info_freer(body->aux.info);
    }
    free(body);
}

void body_remove(body_t *body)
{
    body->aux.is_removed = true;
}

bool body_is_removed(body_t *body)
{
    return body->aux.is_removed;
}

void body_set_movable(body_t *body, bool movable)
{
    body->physical_properties.movable = movable;
}

bool body_is_movable(body_t *body)
{
    return body->physical_properties.movable;
}

void body_set_camera_movement(body_t *body, vector_t movement)
{
    body->aux.camera_movement = movement;
}

void body_set_camera_mode(body_t *body, camera_mode_t camera_mode)
{
    body->aux.camera_mode = camera_mode;
}

camera_mode_t body_get_camera_mode(body_t *body)
{
    return body->aux.camera_mode;
}

list_t *body_get_shape(body_t *body)
{
    list_t *shape = body->appearance.shape;
    list_t *new_shape = list_init(list_size(shape), (free_func_t)vec_free);
    vector_t *current_vector;
    for (size_t idx = 0; idx < list_size(shape); idx++)
    {
        current_vector = (vector_t *)list_get(shape, idx);
        list_add(new_shape, vec_malloc(current_vector->x, current_vector->y));
    }
    return new_shape;
}

vector_t body_get_centroid(body_t *body)
{
    return body->kinematic_variables.position;
}

vector_t body_get_velocity(body_t *body)
{
    return body->kinematic_variables.velocity;
}

double body_get_angular_velocity(body_t *body)
{
    return body->kinematic_variables.angular_velocity;
}

rgb_color_t body_get_color(body_t *body)
{
    return body->appearance.color;
}

double body_get_mass(body_t *body)
{
    return body->physical_properties.mass;
}

void body_set_centroid(body_t *body, vector_t x)
{
    vector_t x0 = body->kinematic_variables.position;
    vector_t dx = vec_subtract(x, x0);
    polygon_translate(body->appearance.shape, dx);
    body->kinematic_variables.position = x;
}

void body_set_color(body_t *body, rgb_color_t color)
{
    body->appearance.color = color;
}

void body_set_velocity(body_t *body, vector_t v)
{
    body->kinematic_variables.velocity = v;
}

void body_set_angular_velocity(body_t *body, double av)
{
    body->kinematic_variables.angular_velocity = av;
}

void body_set_rotation(body_t *body, double angle)
{
    double a0 = body->kinematic_variables.angular_position;
    double da = angle - a0;
    vector_t position = body->kinematic_variables.position;
    polygon_rotate(body->appearance.shape, da, position);
    body->kinematic_variables.angular_position = angle;
}

void body_add_force(body_t *body, vector_t force)
{
    vector_t old_force = body->kinematic_variables.net_force;
    vector_t new_force = vec_add(old_force, force);
    body->kinematic_variables.net_force = new_force;
}

void body_add_impulse(body_t *body, vector_t impulse)
{
    vector_t old_impulse = body->kinematic_variables.impulse;
    vector_t new_impulse = vec_add(old_impulse, impulse);
    body->kinematic_variables.impulse = new_impulse;
}

double body_get_rotation(body_t *body)
{
    return body->kinematic_variables.angular_position;
}

vector_t body_get_force(body_t *body)
{
    return body->kinematic_variables.net_force;
}

void body_tick(body_t *body, double dt)
{
    vector_t force = body->kinematic_variables.net_force;

    double mass = body->physical_properties.mass;
    double inverse_mass = mass == 0 ? 0 : 1.0 / mass;
    vector_t accel = vec_multiply(inverse_mass, force);
    vector_t dv_force = vec_multiply(dt, accel);
    body->kinematic_variables.net_force = VEC_ZERO;

    vector_t dp = body->kinematic_variables.impulse;
    vector_t dv_impulse = vec_multiply(inverse_mass, dp);
    body->kinematic_variables.impulse = VEC_ZERO;

    vector_t dv = vec_add(dv_force, dv_impulse);

    vector_t vi = body_get_velocity(body);
    vector_t vf = vec_add(vi, dv);
    body_set_velocity(body, vf);
    vector_t v_avg = vec_multiply(0.5, vec_add(vi, vf));

    vector_t xi = body_get_centroid(body);
    vector_t dx = vec_multiply(dt, v_avg);
    vector_t xf = vec_add(xi, dx);
    body_set_centroid(body, xf);

    double dtheta = body_get_angular_velocity(body);
    double thetai = body_get_rotation(body);
    double thetaf = thetai + dtheta;
    body_set_rotation(body, thetaf);
}

void body_adjust_camera(body_t *body)
{
    vector_t xi = body_get_centroid(body);
    vector_t dx_cam = body->aux.camera_movement;
    vector_t xf = vec_add(xi, dx_cam);
    body->aux.camera_movement = VEC_ZERO;
    body_set_centroid(body, xf);
}
