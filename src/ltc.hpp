#pragma once

#include <array>
#include "brdf.hpp"

// Note that we are fitting 3 parameters, but saving 5, plus information
// about original distribution norm and Fresnel term.
struct ltc_store_data
{
  std::array<float, 5> matrix_parameters;
  float distribution_norm;
  float fresnel_term;
};

class ltc: public brdf
{
public:
  ltc();

  virtual ~ltc()
  {
  }

  virtual std::string get_name() const
  {
    return "ltc";
  }

  virtual float evaluate(
    const glm::vec3 &light_dir,
    const glm::vec3 &view_dir,
    float &probability_density_function
  ) const;

  virtual glm::vec3 sample(const glm::vec3 &view_dir, const glm::vec2 &random_parameters) const;

  /*
   * Note about matrix and it's inverse. Here I assume that uninversed matrix
   * transformes vector from original (cosine) distribution to another one.
   */
  void set_base_frame(const glm::mat3 &base_frame);

  glm::mat3 get_base_frame() const
  {
    return _base_frame;
  }

  void set_ltc_parameters(const glm::vec3 &parameters);
  void set_ltc_matrix(const glm::mat3 &mat);
  const glm::mat3 &get_ltc_matrix() const;
  const glm::mat3 &get_ltc_matrix_inv() const;
  glm::mat3 get_framed_ltc_matrix() const;
  glm::mat3 get_framed_ltc_matrix_inv() const;

  void set_amplitude(float amplitude);
  float get_amplitude() const;

  void set_fresnel(float fresnel)
  {
    _fresnel = fresnel;
  }

  float get_fresnel() const
  {
    return _fresnel;
  }

  // Store data doesn't hold information about current frame.
  // Saving store data and restoring it later may cause data loss.
  // Frame is already taken into account when generating final parameters.
  ltc_store_data get_store_data() const;
  void set_store_data(const ltc_store_data &data);

private:
  float _amplitude;
  float _fresnel;
  float _ltc_matrix_inv_determinant;
  glm::mat3 _base_frame;
  glm::mat3 _ltc_matrix;
  glm::mat3 _ltc_matrix_inv;
};
