#pragma once

#include "../numerical/constraints_calculator.hpp"

class ltc_constraints_calculator: public constraints_calculator
{
  ltc_constraints_calculator() = default;
  ~ltc_constraints_calculator() override = default;

  std::vector<float> calculate_constraints(const std::vector<float>& parameters) override;
};
