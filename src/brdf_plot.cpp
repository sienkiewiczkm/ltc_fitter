#include "brdf_plot.hpp"
#include "stb_image_write.h"
#include "glm/gtc/matrix_transform.hpp"
#include "raycasting.hpp"
#include <vector>

brdf_plot::brdf_plot():
    _size{512}
{
}

void brdf_plot::set_resolution(int size)
{
    _size = size;
}

void brdf_plot::export_png(
    const brdf* brdf,
    const std::string& filepath
)
{
    _brdf = brdf;
    _max_hotness = 0.0f;

    setup_camera();
    auto image_data = get_image_data();
    save_png(image_data, {_size, _size}, filepath);
}

void brdf_plot::setup_camera()
{
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
}

std::vector<unsigned char> brdf_plot::get_image_data()
{
    std::vector<unsigned char> image_data;
    image_data.resize(3 * _size * _size);

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

    return image_data;
}

void brdf_plot::save_png(
    const std::vector<unsigned char>& image_data,
    const glm::ivec2& resolution,
    const std::string filename
)
{
    int stride = 3 * sizeof(unsigned char) * resolution.x;
    stbi_write_png(
        filename.c_str(),
        resolution.x,
        resolution.y,
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

    glm::vec3 view_dir{-1.0f, 1.0f, 1.0f};
    view_dir = glm::normalize(view_dir);

    auto sphere_direction = glm::normalize(intersection_sphere);

    float pdf;
    auto value = _brdf->evaluate(sphere_direction, view_dir, pdf);
    _max_hotness = std::max(_max_hotness, value);

    auto float_color = glm::vec3{get_hotness_color(value, 0.0f, 5.0f)};

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

glm::vec3 brdf_plot::get_hotness_color(float v, float vmin, float vmax) const
{
    // source, credit to StackOverflow:
    //   https://stackoverflow.com/a/7811134/3495132
    glm::vec3 c = {1.0,1.0,1.0}; // white
    float dv;

    if (v < vmin)
        v = vmin;
    if (v > vmax)
        v = vmax;
    dv = vmax - vmin;

    if (v < (vmin + 0.25 * dv)) {
        c.r = 0;
        c.g = 4 * (v - vmin) / dv;
    } else if (v < (vmin + 0.5 * dv)) {
        c.r = 0;
        c.b = 1 + 4 * (vmin + 0.25 * dv - v) / dv;
    } else if (v < (vmin + 0.75 * dv)) {
        c.r = 4 * (v - vmin - 0.5 * dv) / dv;
        c.b = 0;
    } else {
        c.g = 1 + 4 * (vmin + 0.75 * dv - v) / dv;
        c.b = 0;
    }

    return(c);
}
