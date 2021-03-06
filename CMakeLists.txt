cmake_minimum_required(VERSION 3.8)
project(ltc_fitter)

include_directories(${PROJECT_SOURCE_DIR}/dependencies/glm)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/stb)

add_executable(${PROJECT_NAME}
  src/brdf/brdf.hpp
  src/brdf/ggx.cpp
  src/brdf/ggx.hpp
  src/exporters/alf_exporter.cpp src/exporters/alf_exporter.hpp
  src/files/arealights_ltc_file/arealights_ltc_file.cpp
  src/files/arealights_ltc_file/arealights_ltc_file.hpp
  src/fitting/fitting_result.cpp
  src/fitting/fitting_result.hpp
  src/fitting/ltc_fitting.cpp
  src/fitting/ltc_fitting.hpp
  src/fitting/ltc_lookup_builder.cpp
  src/fitting/ltc_lookup_builder.hpp
  src/fitting_settings.cpp
  src/fitting_settings.hpp
  src/ltc/ltc.cpp
  src/ltc/ltc.hpp
  src/ltc/ltc_constraints_calculator.cpp
  src/ltc/ltc_constraints_calculator.hpp
  src/ltc/ltc_error_estimator.cpp
  src/ltc/ltc_error_estimator.hpp
  src/main.cpp
  src/numerical/constraints_calculator.hpp
  src/numerical/error_estimator.hpp
  src/numerical/logarithmic_penalty_error_estimator.cpp
  src/numerical/logarithmic_penalty_error_estimator.hpp
  src/numerical/nelder_mead.cpp
  src/numerical/nelder_mead.hpp
  src/numerical/penalty_error_estimator.hpp
  src/numerical/penalty_optimizer.cpp
  src/numerical/penalty_optimizer.hpp
  src/numerical/samplers/halton_sampler2d.cpp
  src/numerical/samplers/halton_sampler2d.hpp
  src/numerical/samplers/sampler2d.hpp
  src/numerical/samplers/uniform_sampler2d.cpp
  src/numerical/samplers/uniform_sampler2d.hpp
  src/numerical/samplers/van_der_corput.cpp
  src/numerical/samplers/van_der_corput.hpp
  src/numerical/vector_utils.cpp
  src/numerical/vector_utils.hpp
  src/plotting/brdf_plot.cpp
  src/plotting/brdf_plot.hpp
  src/plotting/raycasting.cpp
  src/plotting/raycasting.hpp
  src/preview.cpp
  src/preview.hpp
  src/tests/export_original_fit.cpp
  src/tests/export_original_fit.hpp
  src/tests/original_heitz_fit.hpp
  src/tests/tests.cpp
  src/tests/tests.hpp
  src/utils/hacks.cpp
  src/utils/hacks.hpp
  src/utils/helpers.cpp
  src/utils/helpers.hpp
  src/utils/log.cpp
  src/utils/log.hpp
  src/utils/stb_implementation.c
  src/utils/constants.hpp)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
