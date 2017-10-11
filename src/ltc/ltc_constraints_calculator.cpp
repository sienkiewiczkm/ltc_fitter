#include "ltc_constraints_calculator.hpp"
#include "../utils/log.hpp"
#include <iostream>

std::vector<float> ltc_constraints_calculator::calculate_constraints(const std::vector<float> &parameters)
{
  auto constraints = std::vector<float>(2);

  if (parameters.size() != 3)
  {
    log_error() << "Expected 3 parameters in ltc_constraints_calculator, received " << parameters.size() << "."
      << std::endl;

    throw std::logic_error("Expected 3 parameters.");
  }

  float parameter_min_value = 1e-6f;

  for (auto i = 0; i < 2; ++i)
  {
    constraints[i] = parameter_min_value - parameters[i];
  }

  return constraints;
}
