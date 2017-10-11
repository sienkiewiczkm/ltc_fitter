#include <iostream>
#include "vector_utils.hpp"
#include "nelder_mead.hpp"
#include "../utils/log.hpp"

std::vector<float> vector_utils::add(const std::vector<float> &a, const std::vector<float> &b)
{
  if (a.size() != b.size())
  {
    log_error() << "[Nelder-Mead] Add operands dimmensions do not match. "
      << "dim(a)=" << a.size() << "; dim(b)=" << b.size();
    throw std::invalid_argument("Operands dimmensions do not match");
  }

  std::vector<float> output(a.size());

  for (auto i = 0; i < a.size(); ++i)
  {
    output[i] = a[i] + b[i];
  }

  return output;
}

std::vector<float> vector_utils::sub(const std::vector<float>& a, const std::vector<float>& b)
{
  if (a.size() != b.size())
  {
    log_error() << "[Nelder-Mead] Sub operands dimmensions do not match. "
      << "dim(a)=" << a.size() << "; dim(b)=" << b.size();
    throw std::invalid_argument("Operands dimmensions do not match");
  }

  std::vector<float> output(a.size());

  for (auto i = 0; i < a.size(); ++i)
  {
    output[i] = a[i] - b[i];
  }

  return output;
}

std::vector<float> vector_utils::mul(const std::vector<float>& a, float multipler)
{
  std::vector<float> output(a.size());

  for (auto i = 0; i < a.size(); ++i)
  {
    output[i] = a[i] * multipler;
  }

  return output;
}

std::vector<float> vector_utils::div(const std::vector<float>& a, float divisor)
{
  std::vector<float> output(a.size());

  for (auto i = 0; i < a.size(); ++i)
  {
    output[i] = a[i] / divisor;
  }

  return output;
}

float vector_utils::sum(const std::vector<float>& a)
{
  float accumulator = 0.0f;

  for (auto i = 0; i < a.size(); ++i)
  {
    accumulator += a[i];
  }

  return accumulator;
}

std::vector<float> vector_utils::abs(const std::vector<float>& input)
{
  std::vector<float> output(input.size());

  for (auto i = 0; i < input.size(); ++i)
  {
    output[i] = ::abs(input[i]);
  }

  return output;
}