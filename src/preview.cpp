#include "preview.hpp"

#include <sstream>
#include <iostream>

#include "fitting/ltc_lookup_builder.hpp"
#include "files/arealights_ltc_file/arealights_ltc_file.hpp"
#include "utils/log.hpp"
#include "plotting/brdf_plot.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

void create_preview_for_file(std::string filename)
{
  auto file_contents = alf::read_alf_file(filename);

  fitting_result result;
  result.resize(file_contents->header.num_samples_angle, file_contents->header.num_samples_roughness);

  log_info() << "File loaded." << std::endl;
  log_info() << "  Num. samples for angle: " << file_contents->header.num_samples_angle << std::endl;
  log_info() << "  Num. samples for roughness: " << file_contents->header.num_samples_roughness << std::endl;

  for (auto theta = 0; theta < file_contents->header.num_samples_angle; ++theta)
  {
    for (auto alpha = 0; alpha < file_contents->header.num_samples_roughness; ++alpha)
    {
      auto matrix_index = file_contents->header.num_samples_roughness * theta + alpha;
      auto base_tex1 = 4 * matrix_index;
      auto base_tex2 = 3 * matrix_index;

      ltc_store_data params;

      params.matrix_parameters[0] = file_contents->texture_slot1[base_tex1+0];
      params.matrix_parameters[1] = file_contents->texture_slot1[base_tex1+1];
      params.matrix_parameters[2] = file_contents->texture_slot1[base_tex1+2];
      params.matrix_parameters[3] = file_contents->texture_slot1[base_tex1+3];

      params.matrix_parameters[4] = file_contents->texture_slot2[base_tex2+0];
      params.distribution_norm    = file_contents->texture_slot2[base_tex2+1];
      params.fresnel_term         = file_contents->texture_slot2[base_tex2+2];

      result.setData(theta, alpha, params);
    }
  }

  for (auto rough_frag = 0; rough_frag < result.num_samples_roughness; ++rough_frag)
  {
    for (auto angle_frag = 0; angle_frag < result.num_samples_angle; ++angle_frag)
    {
      const auto angle_perc = angle_frag / static_cast<float>(result.num_samples_angle - 1);
      float cos_theta = 1.0f - angle_perc * angle_perc;
      float theta = std::min(1.57f, acosf(cos_theta));
      glm::vec3 view_dir{sinf(theta), 0.0f, cosf(theta)};

      brdf_plot plotter;
      plotter.set_view_dir(view_dir);
      plotter.set_resolution(256);

      std::stringstream ss;
      ss << "preview_r" << rough_frag << "_a" << angle_frag << "_";

      ltc ltc;
      auto store_data = result.getData(angle_frag, rough_frag);
      ltc.set_store_data(store_data);

      log_info() << "Store data for (roughness:" << rough_frag << ", theta:" << angle_frag << "):" << std::endl;
      log_info() << "  transform[0] = " << store_data.matrix_parameters[0] << std::endl;
      log_info() << "  transform[1] = " << store_data.matrix_parameters[1] << std::endl;
      log_info() << "  transform[2] = " << store_data.matrix_parameters[2] << std::endl;
      log_info() << "  transform[3] = " << store_data.matrix_parameters[3] << std::endl;
      log_info() << "  transform[4] = " << store_data.matrix_parameters[4] << std::endl;
      log_info() << "          norm = " << store_data.distribution_norm << std::endl;
      log_info() << "       fresnel = " << store_data.fresnel_term << std::endl;
      log_info() << "View direction" << glm::to_string(view_dir) << std::endl;

      try
      {
        plotter.export_png(&ltc, ss.str() + "ltc.png");
      }
      catch (const std::exception& exc)
      {
        log_error() << "Failed to generate image for (roughness:" << rough_frag << ", theta:" << angle_frag << ")."
          << std::endl;
      }
    }
  }
}