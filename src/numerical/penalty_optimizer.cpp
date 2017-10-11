#include "penalty_optimizer.hpp"
#include "nelder_mead.hpp"
#include "../log.hpp"
#include "vector_utils.hpp"
#include <iostream>

std::vector<float> penalty_optimizer::optimize(const std::vector<float>& start_parameters) const
{
  float penalty_value = 10.0f;
  const float penalty_multipler = 0.1f;

  nelder_mead mead_optimizer(start_parameters.size());

  auto current_parameters = start_parameters;
  const float acceptable_step_norm = 0.01f;

  const int max_iterations = 10;
  int iteration = 0;

  for (; iteration < max_iterations; ++iteration)
  {
    _estimator->set_penalty_factor(penalty_value);
    auto new_parameters = mead_optimizer.optimize(current_parameters, _estimator.get());

    auto norm = vector_utils::sum(vector_utils::abs(vector_utils::sub(current_parameters, new_parameters)));
    
    current_parameters = new_parameters;

    if (norm < acceptable_step_norm)
    {
      break;
    }

    penalty_value *= penalty_multipler;
  }

  if (iteration >= max_iterations)
  {
    log_info() << "Maximum number of iterations reached." << std::endl;
  }

  return current_parameters;
}
