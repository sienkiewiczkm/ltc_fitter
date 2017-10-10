#pragma once

#include <string>
#include "glm/glm.hpp"
#include "brdf.hpp"

class brdf_plot
{
public:
  brdf_plot();

  ~brdf_plot()
  {
  }

  void set_view_dir(const glm::vec3 &view_dir);
  void set_resolution(int size);
  void export_png(const brdf *brdf, const std::string &filepath);

protected:
  void setup_camera();
  std::vector<unsigned char> get_image_data();
  float estimate_maximum_value(int samples_sqrt = 128);
  void save_png(const std::vector<unsigned char> &image_data, const glm::ivec2 &resolution, const std::string filename);

  glm::vec3 get_pixel_color(glm::vec2 screen_coord);
  glm::vec3 get_ray_origin(glm::vec2 screen_coord);
  glm::vec3 get_ray_direction(glm::vec2 screen_coord);
  glm::ivec3 get_8bit_color(glm::vec3 float_color);
  glm::vec3 get_hotness_color(float v, float vmin, float vmax) const;

private:
  int _size;

  const brdf *_brdf;
  glm::vec3 _eye_position;
  glm::vec3 _eye_target;
  glm::vec3 _sphere_origin;
  float _sphere_radius;
  float _max_hotness;
  glm::vec3 _view_dir;
  glm::mat4 _view_matrix_inv;
};
