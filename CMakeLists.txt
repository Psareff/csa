cmake_minimum_required(VERSION 3.10)
project(HTTPServer)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  libhv
  GIT_REPOSITORY https://github.com/ithewei/libhv.git
  GIT_TAG        master
)
FetchContent_Declare(
  nlohmann_json
  GIT_REPOSITORY https://github.com/nlohmann/json.git
  GIT_TAG        master
)

FetchContent_MakeAvailable(libhv nlohmann_json)

add_executable(HTTPServer src/server.cpp src/Utility.cpp)
add_executable(HTTPClient src/client.cpp src/Utility.cpp)

target_link_libraries(HTTPServer hv_static nlohmann_json::nlohmann_json)
target_link_libraries(HTTPClient hv_static nlohmann_json::nlohmann_json)
