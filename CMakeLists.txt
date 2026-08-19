cmake_minimum_required(VERSION 4.3)
project(warthunder_toolkit)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.51.0
)
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(httplib json)

add_executable(warthunder_toolkit src/main.cpp)
target_link_libraries(warthunder_toolkit PRIVATE httplib::httplib nlohmann_json::nlohmann_json)