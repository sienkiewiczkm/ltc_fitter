cmake_minimum_required(VERSION 3.8)
project(ltc_fitter)

find_package(
        Boost 1.63.0 REQUIRED COMPONENTS program_options date_time
)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${PROJECT_SOURCE_DIR}/dependencies/glm)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/stb)

add_executable(
        ${PROJECT_NAME}
        src/brdf.hpp
        src/brdf_plot.cpp
        src/brdf_plot.hpp
        src/fitting_settings.cpp
        src/fitting_settings.hpp
        src/ggx.cpp
        src/ggx.hpp
        src/hdr_exporter.cpp
        src/hdr_exporter.hpp
        src/helpers.cpp
        src/helpers.hpp
        src/log.cpp
        src/log.hpp
        src/ltc.cpp
        src/ltc.hpp
        src/ltc_fitting.cpp
        src/ltc_fitting.hpp
        src/ltc_lookup_builder.cpp
        src/ltc_lookup_builder.hpp
        src/ltc_nelder_mead.cpp
        src/ltc_nelder_mead.hpp
        src/main.cpp
        src/nelder_mead.cpp
        src/nelder_mead.hpp
        src/raycasting.cpp
        src/raycasting.hpp
        src/result_saving.cpp
        src/result_saving.hpp
        src/stb_implementation.c
        src/tests.cpp
        src/tests.hpp
)

target_link_libraries(
        ${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES}
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
