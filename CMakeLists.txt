cmake_minimum_required(VERSION 2.8)

project(tatCom)
add_executable(tatCom main.cxx)
find_package(SFML REQUIRED system window graphics network audio)
target_link_libraries(tatCom ${SFML_LIBRARIES})
