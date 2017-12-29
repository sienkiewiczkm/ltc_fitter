#pragma once

#include <vector>
#include "../fitting_settings.hpp"
#include "../ltc/ltc.hpp"

struct fitting_result
{
  unsigned int num_samples_angle;
  unsigned int num_samples_roughness;
  std::vector<ltc_store_data> transformations;

  void resize(unsigned int num_angle_samples, unsigned int num_roughness_samples);
  void setData(unsigned int angle, unsigned int roughness, const ltc_store_data& data);
  const ltc_store_data& getData(unsigned int angle, unsigned int roughness) const;
  int getDataIndex(unsigned int angle, unsigned int roughness) const;
};
