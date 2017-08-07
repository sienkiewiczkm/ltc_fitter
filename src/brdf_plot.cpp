#include "brdf_plot.hpp"
#include "stb_image_write.h"
#include "glm/gtc/matrix_transform.hpp"
#include "raycasting.hpp"
#include <vector>

brdf_plot::brdf_plot():
    _size{256}
{
}

void brdf_plot::set_resolution(int size)
{
    _size = size;
}

void brdf_plot::export_png(
    const std::string& filepath
)
{
    std::vector<unsigned char> image_data;
    image_data.resize(3 * _size * _size);

    _sphere_origin = {0.0, 0.0f, 0.0f};
    _sphere_radius = 1.0f;

    _eye_position = {0.0f, 1.0f, -1.4f};
    _eye_target = {0.0f, 0.0f, 0.0f};

    glm::mat4 view_matrix = glm::lookAt(
        _eye_position,
        _sphere_origin,
        glm::vec3{0.0f, 1.0f, 0.0f}
    );

    _view_matrix_inv = glm::inverse(view_matrix);

    for (auto y = 0; y < _size; ++y)
    {
        float dy = 1.0f-(static_cast<float>(y+0.5f) / _size);

        for (auto x = 0; x < _size; ++x)
        {
            float dx = static_cast<float>(x+0.5f) / _size;

            auto float_color = get_pixel_color({dx, dy});
            auto color = get_8bit_color(float_color);

            auto index = 3 * (y * _size + x);
			image_data[index+0] = static_cast<unsigned char>(color.r);
			image_data[index+1] = static_cast<unsigned char>(color.g);
			image_data[index+2] = static_cast<unsigned char>(color.b);
        }
    }

    int stride = 3 * sizeof(unsigned char) * _size;
    stbi_write_png(
        filepath.c_str(),
        _size,
        _size,
        3,
        image_data.data(),
        stride
    );
}

glm::vec3 brdf_plot::get_pixel_color(glm::vec2 screen_coord)
{
    auto ray_origin = get_ray_origin(screen_coord);
    auto ray_direction = get_ray_direction(screen_coord);

    float distance = 0.0f;
    auto hit = ray_sphere_min_intersection(
        ray_origin,
        ray_direction,
        _sphere_origin,
        _sphere_radius,
        distance
    );

    if (!hit) { return {1.0f, 1.0f, 1.0f}; }

    auto intersection = ray_origin + distance * ray_direction;
    auto intersection_sphere = intersection - _sphere_origin;

    if (intersection_sphere.y < 0.0f) { return {1.0f, 1.0f, 1.0f}; }

    auto sphere_direction = glm::normalize(intersection_sphere);
    auto float_color = glm::vec3{sphere_direction.y, 0.0f, 0.0f};
    float_color = float_color * float_color;

    return float_color;
}

//define USE_PERSPECTIVE

glm::vec3 brdf_plot::get_ray_origin(glm::vec2 screen_coord)
{
#ifdef USE_PERSPECTIVE
    return _eye_position;
#else
    float x_plane_size = 1.1f;
    float y_plane_size = 1.1f;

    auto view_ray_origin = glm::vec3{
        x_plane_size * (screen_coord.x * 2.0f - 1.0f),
        y_plane_size * (screen_coord.y * 2.0f - 1.0f),
        0.0f
    };

    auto ray_origin = glm::vec3{
        _view_matrix_inv * glm::vec4{view_ray_origin, 1.0f}
    };

    return ray_origin;
#endif
}

glm::vec3 brdf_plot::get_ray_direction(glm::vec2 screen_coord)
{
#ifdef USE_PERSPECTIVE
    float fov_radians = glm::radians(90.0f);
    float x_plane_size = std::tan(fov_radians/2.0f);
    float y_plane_size = std::tan(fov_radians/2.0f);

    auto view_ray_direction = glm::normalize(glm::vec3{
        x_plane_size * (screen_coord.x * 2.0f - 1.0f),
        y_plane_size * (screen_coord.y * 2.0f - 1.0f),
        -1.0f
    });

    auto ray_direction = glm::vec3{
        _view_matrix_inv * glm::vec4{view_ray_direction, 0.0f}
    };

    return glm::normalize(ray_direction);
#else
    return glm::normalize(_eye_target - _eye_position);
#endif
}

glm::ivec3 brdf_plot::get_8bit_color(glm::vec3 float_color)
{
    auto clamped_color = glm::clamp(float_color, 0.0f, 1.0f);
    auto r = std::roundf(255.0f * clamped_color.r);
    auto g = std::roundf(255.0f * clamped_color.g);
    auto b = std::roundf(255.0f * clamped_color.b);
    return glm::ivec3{r, g, b};

}
