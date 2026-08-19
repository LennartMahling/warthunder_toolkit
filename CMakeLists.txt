cmake_minimum_required(VERSION 3.20)
project(warthunder_toolkit)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

# 1. CPP-HTTPLIB
FetchContent_Declare(
        httplib
        GIT_REPOSITORY https://github.com/yhirose/cpp-httplib.git
        GIT_TAG v0.51.0
)

# 2. JSON-Bibliothek
FetchContent_Declare(
        json
        GIT_REPOSITORY https://github.com/nlohmann/json.git
        GIT_TAG v3.12.0
)

# 3. GoogleTest (Mit fester Versionsnummer für stabiles Laden)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.18.0  # <-- Geändert von 'main' auf eine feste Version
)

# Alle Pakete gleichzeitig herunterladen und aktivieren
FetchContent_MakeAvailable(httplib json googletest)

enable_testing()

# ==========================================
# Hauptprogramm
# ==========================================
add_executable(warthunder_toolkit
        src/main.cpp
        src/TelemetryFrame.cpp
        src/TelemetryClient.cpp
        src/TelemetryLogger.cpp
)
target_link_libraries(warthunder_toolkit PRIVATE httplib::httplib nlohmann_json::nlohmann_json)

# ==========================================
# Tests (isoliert, ohne Netzwerk/Dateisystem)
# ==========================================
add_executable(warthunder_toolkit_tests
        tests/TelemetryFrameTests.cpp
        src/TelemetryFrame.cpp
)
target_link_libraries(warthunder_toolkit_tests PRIVATE gtest_main nlohmann_json::nlohmann_json)

# Suchpfad für Header-Dateien (.h) in den Tests aktivieren
target_include_directories(warthunder_toolkit_tests PRIVATE src)

# Automatische Test-Erkennung für CLion registrieren
include(GoogleTest)
gtest_discover_tests(warthunder_toolkit_tests)
