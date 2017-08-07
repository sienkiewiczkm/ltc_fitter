#pragma once
#include <string>
#include "brdf.hpp"

class brdf_plot
{
public:
    brdf_plot();
    ~brdf_plot() {}

    void set_resolution(int size);
    void export_png(const std::string& filepath);

protected:
    glm::vec3 get_pixel_color(glm::vec2 screen_coord);
    glm::vec3 get_ray_origin(glm::vec2 screen_coord);
    glm::vec3 get_ray_direction(glm::vec2 screen_coord);

    glm::ivec3 get_8bit_color(glm::vec3 float_color);

private:
    int _size;

    glm::vec3 _eye_position;
    glm::vec3 _eye_target;
    glm::vec3 _sphere_origin;
    float _sphere_radius;

    glm::mat4 _view_matrix_inv;
};
