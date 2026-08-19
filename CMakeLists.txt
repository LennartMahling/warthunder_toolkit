cmake_minimum_required(VERSION 3.20)

project(warthunder_toolkit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.53.1
)

FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)

FetchContent_MakeAvailable(httplib json)

add_executable(warthunder_toolkit
        src/main.cpp
        src/TelemetryFrame.cpp
        src/TelemetryClient.cpp
        src/TelemetryLogger.cpp

)

target_link_libraries(warthunder_toolkit PRIVATE httplib::httplib nlohmann_json::nlohmann_json)


add_executable(warthunder_toolkit_tests
        src/TelemetryFrame.cpp
)
target_link_libraries(warthunder_toolkit_tests PRIVATE gtest_main nlohmann_json::nlohmann_json)
include(GoogleTest)
gtest_discover_tests(warthunder_toolkit_tests)