#include "fitting_result.hpp"

void fitting_result::resize(unsigned int num_angle_samples, unsigned int num_roughness_samples)
{
  this->num_samples_angle = num_angle_samples;
  this->num_samples_roughness = num_roughness_samples;
  transformations.resize(num_angle_samples * num_roughness_samples);
}

void fitting_result::setData(unsigned int angle, unsigned int roughness, const ltc_store_data& data)
{
  transformations[getDataIndex(angle, roughness)] = data;
}

const ltc_store_data& fitting_result::getData(unsigned int angle, unsigned int roughness) const
{
  return transformations[getDataIndex(angle, roughness)];
}

int fitting_result::getDataIndex(unsigned int angle, unsigned int roughness) const
{
  return num_samples_roughness * angle + roughness;
}