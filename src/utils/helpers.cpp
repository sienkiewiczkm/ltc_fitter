#include "helpers.hpp"
#include <string>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string get_time_str()
{
  // Credit to: https://stackoverflow.com/a/17223443
  auto current_time = std::chrono::system_clock::now();
  std::time_t t = std::chrono::system_clock::to_time_t(current_time);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&t), "%Y-%m-%d %X");
  return ss.str();
}
