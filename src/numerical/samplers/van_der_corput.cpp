#include "van_der_corput.hpp"

double van_der_corput(uint32_t n, uint32_t base)
{
  double result = 0, denominator = 1;
  while (n)
  {
    denominator *= base;
    result += (n % base) / denominator;
    n /= base;
  }
  return result;
}