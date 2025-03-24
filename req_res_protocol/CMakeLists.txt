cmake_minimum_required(VERSION 3.20)
project(req_res_protocol VERSION 1.0)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable(pserver "${PROJECT_SOURCE_DIR}/src/server.cpp")
add_executable(pclient "${PROJECT_SOURCE_DIR}/src/client.cpp")

# add_library(mylib SHARED "${PROJECT_SOURCE_DIR}/src/lib.cpp")
# target_link_libraries(pserver mylib)
# target_link_libraries(pclient mylib)
