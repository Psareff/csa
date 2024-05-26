cmake_minimum_required(VERSION 3.10)
project(MyHTTPServer)

set(CMAKE_CXX_STANDARD 17)

# Add dependencies
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

# Add the executable
add_executable(MyHTTPServer src/main.cpp src/Utility.cpp)

# Link libraries
target_link_libraries(MyHTTPServer hv_static nlohmann_json::nlohmann_json)
