cmake_minimum_required(VERSION 3.8)
project(ltc_fitter)

find_package(Boost 1.63.0 REQUIRED COMPONENTS program_options date_time)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/dependencies/glm)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/stb)

add_executable(${PROJECT_NAME}
  src/brdf/brdf.hpp
  src/brdf/ggx.cpp
  src/brdf/ggx.hpp
  src/exporters/hdr_exporter.cpp
  src/exporters/hdr_exporter.hpp
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
  src/numerical/vector_utils.cpp
  src/numerical/vector_utils.hpp
  src/plotting/brdf_plot.cpp
  src/plotting/brdf_plot.hpp
  src/plotting/raycasting.cpp
  src/plotting/raycasting.hpp
  src/plotting/result_saving.cpp
  src/plotting/result_saving.hpp
  src/tests/tests.cpp
  src/tests/tests.hpp
  src/utils/helpers.cpp
  src/utils/helpers.hpp
  src/utils/log.cpp
  src/utils/log.hpp
  src/utils/stb_implementation.c
  src/brdf/ggx_slope.cpp src/brdf/ggx_slope.hpp)

target_link_libraries(${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
