#pragma once

#include "error_estimator.hpp"
#include "glm/glm.hpp"
#include <vector>

class nelder_mead
{
public:
  nelder_mead(int space_dimmension);
  virtual ~nelder_mead() = default;

  std::vector<float> optimize(std::vector<float> start_parameters, const error_estimator* estimator);

private:
  static const float _reflection_coeff;
  static const float _expansion_coeff;
  static const float _contraction_coeff;
  static const float _shrink_coeff;

  bool run_iteration();
  void find_min_max_error();
  float get_simplex_norm();
  std::vector<float> get_center_of_mass();
  void reduce_simplex(int around);

  int _space_dimmension;
  float _iteration_epsilon;

  std::vector<std::vector<float>> _simplex;
  float _min_error, _max_error, _almost_max_error;
  int _min_index, _max_index, _almost_max_index;

  const error_estimator* _estimator;
};

