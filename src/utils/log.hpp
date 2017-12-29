#pragma once

#include <ostream>
#include <string>
#include "glm/glm.hpp"

enum log_level
{
  log_level_none = 0,
  log_level_error = 1,
  log_level_warning = 2,
  log_level_info = 3,
  log_level_debug = 4
};

extern log_level g_log_level;

std::ostream &log_debug();
std::ostream &log_info();
std::ostream &log_warning();
std::ostream &log_error();
