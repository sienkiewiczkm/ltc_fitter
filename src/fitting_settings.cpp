#include <iostream>
#include <sstream>
#include "fitting_settings.hpp"

//namespace po = boost::program_options;

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

  /*
  po::options_description description("Allowed options");
  description.add_options()
    ("help", "help message")
    ("test", "run test suite")
    (
      "preview-target",
      po::value<std::string>(&output.input_file),
      "generates images for preview target"
    )
    (
      "resolution,r",
      po::value<int>(&output.resolution)->default_value(64),
      "resolution of output image"
    )
    (
      "minroughness,m",
      po::value<float>(&output.min_roughness)->default_value(0.0001f),
      "minimal roughness, should be greater than 0"
    )
    (
      "maxroughness,M",
      po::value<float>(&output.max_roughness)->default_value(1.0f),
      "maximum roughness"
    )
    (
      "errorsamples,E",
      po::value<int>(
        &output.num_error_estimate_samples
      )->default_value(64),
      "number of samples during error estimation"
    )
    (
      "threads,j",
      po::value<int>(
        &output.num_threads
      )->default_value(1),
      "number of threads"
    )
    (
      "brdf,b",
      po::value<std::string>(&output.brdf_method)->default_value("ggx"),
      "brdf method"
    )
    (
      "output,o",
      po::value<std::string>(&output.output_file),
      "output file"
    );

  po::variables_map var_map;
  po::store(po::parse_command_line(argc, argv, description), var_map);
  po::notify(var_map);

  if (var_map.count("help"))
  {
    std::cout << "LTC Fitter" << std::endl
      << "Part of Master's thesis by Kamil Sienkiewicz" << std::endl
      << "Based on work of Heitz et al: \"Linearly Transformed Cosines\""
      << std::endl;
    std::cout << description << std::endl;

    output.run_mode = run_mode::exit;
    return true;
  }

  if (var_map.count("test"))
  {
    output.run_mode = run_mode::test;
    return true;
  }

  if (var_map.count("preview-target")) {
    output.run_mode = run_mode::preview_input;
    return true;
  }

  if (!var_map.count("output"))
  {
    std::cout << "Output file is not set." << std::endl
      << description << std::endl;
    return false;
  }
  */

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
