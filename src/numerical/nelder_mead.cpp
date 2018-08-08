#include "nelder_mead.hpp"
#include "../utils/log.hpp"
#include "vector_utils.hpp"
#include <iostream>
#include <algorithm>

const float nelder_mead::_reflection_coeff = 1.0f;
const float nelder_mead::_expansion_coeff = 2.0f;
const float nelder_mead::_contraction_coeff = 0.5f;
const float nelder_mead::_shrink_coeff = 0.5f;

nelder_mead::nelder_mead(int space_dimmension):
  _space_dimmension{space_dimmension},
  _iteration_epsilon{0.0001f},
  _simplex{}
{
}

std::vector<float> nelder_mead::optimize(std::vector<float> start_parameters, const error_estimator* estimator)
{
  _estimator = estimator;

  if (start_parameters.size() != _space_dimmension)
  {
    log_error() << "[Nelder-Mead] Start parameters have invalid dimmension (is= " << start_parameters.size()
      << "; required=" << _space_dimmension << ").";

    throw std::invalid_argument("Start parameters have invalid dim.");
  }

  auto x_opt = std::vector<float>(_space_dimmension);

  _simplex.resize(_space_dimmension + 1);

  for (auto &i : _simplex)
  {
    i = start_parameters;
  }

  float delta = 0.1f;
  for (auto i = 1; i <= _space_dimmension; ++i)
  {
    _simplex[i][i - 1] += delta;
  }

  _iteration_epsilon = 1e-5f;

  auto iteration = 0;
  auto max_iterations = 100;

  for (iteration = 0; iteration < max_iterations; ++iteration)
  {
    if (!run_iteration())
    {
      x_opt = _simplex[_min_index];
      break;
    }
  }

  if (iteration >= max_iterations)
  {
    // iteration limit reached
    x_opt = _simplex[_min_index];
    log_warning() << "[Nelder-Mead] Iteration limit reached (" << max_iterations << ")." << std::endl;
  }
  else
  {
    log_info() << "[Nelder-Mead] Solution found after " << iteration + 1 << " iterations." << std::endl;
  }

  return x_opt;
}

bool nelder_mead::run_iteration()
{
  find_min_max_error();

  /*
  if (get_simplex_norm() < _iteration_epsilon)
  {
    return false;
  }
  */

  // stop if difference between maximum and minimum value is very low
  // read the source: Numerical Recipes in C++ (3rd Ed.)
  float a = fabsf(_min_error);
  float b = fabsf(_max_error);

  if (2.0f * fabsf(a - b) < (a + b) * _iteration_epsilon)
  {
    return false;
  }

  auto center_of_mass = get_center_of_mass();

  auto pr = vector_utils::add(center_of_mass, vector_utils::mul(
    vector_utils::sub(center_of_mass, _simplex[_max_index]),
    _reflection_coeff
  ));

  auto pr_value = _estimator->estimate_error(pr);

  // this almost maximum error comparison comes from Wikipedia article
  // refer: 'One possible variation of the NM algorithm' section
  if (pr_value >= _min_error && pr_value < _almost_max_error)
  {
    _simplex[_almost_max_index] = pr;
    return true;
  }
  else if (pr_value < _min_error)
  {
    auto pe = vector_utils::add(center_of_mass, vector_utils::mul(
      vector_utils::sub(pr, center_of_mass),
      _expansion_coeff
    ));

    auto pe_value = _estimator->estimate_error(pe);

    if (pe_value < pr_value) // expansion
    {
      _simplex[_max_index] = pe;
    }
    else // reflection
    {
      _simplex[_max_index] = pr;
    }
  }
  else
  {
    if (_min_error <= pr_value && pr_value <= _max_error) // reflection
    {
      _simplex[_max_index] = pr;
    }
    else
    {
      auto pc = vector_utils::add(center_of_mass, vector_utils::mul(
        vector_utils::sub(_simplex[_max_index], center_of_mass),
        _contraction_coeff
      ));

      auto pc_value = _estimator->estimate_error(pc);

      if (pc_value >= _max_error)
      {
        reduce_simplex(_min_index);
      }
      else // contraction
      {
        _simplex[_max_index] = pc;
      }
    }
  }

  return true;
}

void nelder_mead::find_min_max_error()
{
  _min_error = std::numeric_limits<float>::max();
  _max_error = std::numeric_limits<float>::lowest();
  _almost_max_error = std::numeric_limits<float>::lowest();
  _min_index = _max_index = _almost_max_index = -1;

  for (auto i = 0; i < _simplex.size(); ++i)
  {
    float value = _estimator->estimate_error(_simplex[i]);

    if (_min_error > value)
    {
      _min_error = value;
      _min_index = i;
    }

    if (_max_error < value)
    {
      _almost_max_error = _max_error;
      _almost_max_index = _max_index;
      _max_error = value;
      _max_index = i;
    }
    else if (_almost_max_error < value)
    {
      _almost_max_error = value;
      _almost_max_index = i;
    }
  }
}

float nelder_mead::get_simplex_norm()
{
  auto max_norm = 0.0f;

  for (auto i = 0; i < _simplex.size(); ++i)
  {
    auto current_norm = vector_utils::sum(vector_utils::abs(vector_utils::sub(_simplex[_min_index], _simplex[i])));
    max_norm = std::max(max_norm, current_norm);
  }

  return max_norm;
}

std::vector<float> nelder_mead::get_center_of_mass()
{
  std::vector<float> center_of_mass(_space_dimmension);

  for (auto i = 0; i < _simplex.size(); ++i)
  {
    if (i != _max_index)
    {
      center_of_mass = vector_utils::add(center_of_mass, _simplex[i]);
    }
  }

  center_of_mass = vector_utils::div(center_of_mass, static_cast<float>(_space_dimmension));

  return center_of_mass;
}

void nelder_mead::reduce_simplex(int around)
{
  for (auto i = 0; i < _simplex.size(); ++i)
  {
    if (i != around)
    {
      _simplex[i] = vector_utils::add(_simplex[around], vector_utils::mul(
        vector_utils::sub(_simplex[i], _simplex[around]),
        _shrink_coeff
      ));
    }
  }
}

