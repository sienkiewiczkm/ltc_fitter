#pragma once

#include <vector>

class constraints_calculator
{
public:
  constraints_calculator() = default;
  virtual ~constraints_calculator() = default;
  virtual std::vector<float> calculate_constraints(const std::vector<float>& parameters) = 0;
};
