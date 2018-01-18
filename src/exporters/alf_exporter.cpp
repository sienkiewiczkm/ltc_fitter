#include <memory>
#include <cstring>
#include "alf_exporter.hpp"

std::unique_ptr<alf::alf_file> make_alf_file(const fitting_result &data)
{
  auto file = std::make_unique<alf::alf_file>();

  std::strcpy(file->header.id, "ALF");
  file->header.num_samples_angle = data.num_samples_angle;
  file->header.num_samples_roughness = data.num_samples_roughness;
  file->header.version_major = 1;
  file->header.version_minor = 0;

  auto num_samples = file->header.num_samples_angle * file->header.num_samples_roughness;
  file->texture_slot1.resize(4 * num_samples);
  file->texture_slot2.resize(3 * num_samples);

  for (auto theta = 0; theta < file->header.num_samples_angle; ++theta)
  {
    for (auto alpha = 0; alpha < file->header.num_samples_roughness; ++alpha)
    {
      auto matrix_index = file->header.num_samples_roughness * theta + alpha;
      auto base_tex1 = 4 * matrix_index;
      auto base_tex2 = 3 * matrix_index;

      const auto& params = data.getData(theta, alpha);

      file->texture_slot1[base_tex1+0] = params.matrix_parameters[0];
      file->texture_slot1[base_tex1+1] = params.matrix_parameters[1];
      file->texture_slot1[base_tex1+2] = params.matrix_parameters[2];
      file->texture_slot1[base_tex1+3] = params.matrix_parameters[3];

      file->texture_slot2[base_tex2+0] = params.matrix_parameters[4];
      file->texture_slot2[base_tex2+1] = params.distribution_norm;
      file->texture_slot2[base_tex2+2] = params.fresnel_term;
    }
  }

  return file;
}
