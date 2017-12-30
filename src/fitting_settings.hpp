#pragma once

#include <string>

enum class run_mode {
  normal,
  test,
  preview_input,
  exit
};

struct fitting_settings
{
  fitting_settings():
    run_mode{run_mode::normal}
  {
  }

  run_mode run_mode;
  int resolution;
  float min_roughness;
  float max_roughness;
  int num_error_estimate_samples;
  int num_threads;
  std::string brdf_method;
  std::string output_file;

  std::string input_file;
};

bool get_fitting_settings_from_command_line(
  fitting_settings &output,
  int argc,
  const char *argv[]
);

void print_fitting_settings(const fitting_settings &settings);
