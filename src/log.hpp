#pragma once
#include <ostream>
#include <string>
#include "glm/glm.hpp"

std::ostream& log_info();
std::ostream& log_warning();
std::ostream& log_error();

std::string format_exact_vector(const glm::vec3& vec);
std::string format_exact_vector(const glm::vec4& vec);
