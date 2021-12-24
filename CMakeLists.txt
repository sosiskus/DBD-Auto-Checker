cmake_minimum_required(VERSION 3.0.0)
project(DBD_Auto_Check VERSION 0.1.0)

find_package( OpenCV REQUIRED )

include_directories(${OpenCV_INCLUDE-DIRECTORIES})

add_executable(DBD_Auto_Check main.cpp)
# target_compile_features(DBD_Auto_Check PRIVATE cxx_std_17)

target_link_libraries( DBD_Auto_Check ${OpenCV_LIBS} )

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
