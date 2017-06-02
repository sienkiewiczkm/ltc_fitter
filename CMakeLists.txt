cmake_minimum_required(VERSION 3.8)
project(ltc_fitter)

find_package(
    Boost 1.63.0 REQUIRED COMPONENTS program_options
)

include_directories(${PROJECT_SOURCE_DIR}/dependencies/glm)
include_directories(${PROJECT_SOURCE_DIR}/dependencies/stb)

add_executable(
    ${PROJECT_NAME}
    src/main.cpp
    src/fitting_settings.cpp
    src/ltc_fitting.cpp
    src/result_saving.cpp
    src/stb_implementation.c
)

target_link_libraries(
    ${PROJECT_NAME} PUBLIC ${Boost_LIBRARIES}
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_14)
