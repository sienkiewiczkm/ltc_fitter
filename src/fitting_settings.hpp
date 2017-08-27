#pragma once
#include <string>

struct fitting_settings
{
    fitting_settings() {}

    int resolution;
    float min_roughness;
    float max_roughness;
    int num_error_estimate_samples;
    int num_threads;
    std::string brdf_method;
    std::string output_file;
};

bool get_fitting_settings_from_command_line(
    fitting_settings &output,
    int argc,
    const char *argv[]
);

void print_fitting_settings(const fitting_settings& settings);
