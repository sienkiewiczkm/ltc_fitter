#include <iostream>
#include <sstream>
#include <string>
#include "fitting_settings.hpp"


bool get_fitting_settings_from_command_line(
  fitting_settings &output,
  int argc,
  const char *argv[]
)
{
  output = fitting_settings();

  output.brdf_method = "ggx";
  output.resolution = 64;
  output.min_roughness = 0.0001f;
  output.max_roughness = 1.0f;
  output.num_error_estimate_samples = 32;

  std::stringstream ss;
  ss << "result_" << output.resolution << "x" << output.resolution << ".ltc";
  output.output_file = ss.str();

  for (auto argId = 1; argId < argc; ++argId) {
    std::string argument = argv[argId];
    std::cout << "arg: " << argument << std::endl;
    if (argument.substr(0, 2) != "--") {
      std::cerr << "Unrecognized argument \"" << argument << "\". Aborting.";
      throw std::logic_error("Unrecognized argument.");
    }

    auto assignmentOperator = argument.find_first_of('=');
    if (assignmentOperator == -1) {
      throw std::logic_error("Argument assignment has assignment operator.");
    }

    auto key = argument.substr(2, assignmentOperator - 2);
    auto value = argument.substr(assignmentOperator + 1, argument.length() - assignmentOperator - 1);

    if (key == "brdf") {
      output.brdf_method = value;
    }
    else if (key == "resolution") {
      output.resolution = std::stoi(value);
    }
    else if (key == "min-roughness") {
      output.min_roughness = std::stof(value);
    }
    else if (key == "output") {
      output.output_file = value;
    }
  }

  return true;
}

void print_fitting_settings(const fitting_settings &settings)
{
  std::cout << "Current fitting settings: " << std::endl
    << "\tImage resolution:\t" << settings.resolution << "x"
    << settings.resolution << std::endl
    << "\tMinimum roughness:\t" << settings.min_roughness << std::endl
    << "\tMaximum roughness:\t" << settings.max_roughness << std::endl
    << "\tError est. samples:\t" << settings.num_error_estimate_samples
    << std::endl << "\tLookup output file:\t" << settings.output_file
    << std::endl;
}
