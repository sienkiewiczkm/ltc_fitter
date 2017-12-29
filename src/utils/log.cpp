#include "log.hpp"
#include "helpers.hpp"
#include <iostream>
#include <sstream>

log_level g_log_level = log_level_info;
std::stringstream g_sink_stringstream;

std::ostream &log_debug()
{
  if (g_log_level >= log_level_debug)
  {
    return std::cout << get_time_str() << "  DBG ";
  }

  g_sink_stringstream.clear();
  return g_sink_stringstream;
}

std::ostream &log_info()
{
  if (g_log_level >= log_level_info)
  {
    return std::cout << get_time_str() << " INFO ";
  }

  g_sink_stringstream.clear();
  return g_sink_stringstream;
}

std::ostream &log_warning()
{
  if (g_log_level >= log_level_warning)
  {
    return std::cout << get_time_str() << " WARN ";
  }

  g_sink_stringstream.clear();
  return g_sink_stringstream;
}

std::ostream &log_error()
{
  if (g_log_level >= log_level_error)
  {
    return std::cerr << get_time_str() << "  ERR ";
  }

  g_sink_stringstream.clear();
  return g_sink_stringstream;
}
