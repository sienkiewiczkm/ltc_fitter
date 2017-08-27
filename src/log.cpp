#include "log.hpp"
#include "helpers.hpp"
#include <iostream>
#include <sstream>

std::ostream& log_info()
{
    return std::cout << get_time_str() << " INFO ";
}

std::ostream& log_warning()
{
    return std::cout << get_time_str() << " WARN ";
}

std::ostream& log_error()
{
    return std::cerr << get_time_str() << "  ERR ";
}

std::string format_exact_vector(const glm::vec3& vec)
{
    std::stringstream ss;

    ss << "glm::vec3(";
    ss << std::hexfloat << vec.x << ", ";
    ss << std::hexfloat << vec.y << ", ";
    ss << std::hexfloat << vec.z << ")";

    return ss.str();
}

std::string format_exact_vector(const glm::vec4& vec)
{
    std::stringstream ss;

    ss << "glm::vec4(";
    ss << std::hexfloat << vec.x << ", ";
    ss << std::hexfloat << vec.y << ", ";
    ss << std::hexfloat << vec.z << ", ";
    ss << std::hexfloat << vec.w << ")";

    return ss.str();
}
