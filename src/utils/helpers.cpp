#include "helpers.hpp"
#include <string>
#include "boost/date_time/posix_time/posix_time.hpp"

std::string get_time_str()
{
  auto now = boost::posix_time::second_clock::local_time();
  return boost::posix_time::to_simple_string(now);
}
