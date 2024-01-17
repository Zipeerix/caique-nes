########################################################################################################################
#
#   Copyright 2023 CaiqueNES
#
#   Licensed under the Apache License, Version 2.0 (the "License");
#   you may not use this file except in compliance with the License.
#   You may obtain a copy of the License at
#
#   http://www.apache.org/licenses/LICENSE-2.0
#
#   Unless required by applicable law or agreed to in writing, software
#   distributed under the License is distributed on an "AS IS" BASIS,
#   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#   See the License for the specific language governing permissions and
#   limitations under the License.
#
########################################################################################################################

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/release-1.12.0.zip
)
FetchContent_MakeAvailable(googletest)

file(GLOB_RECURSE TEST_FILES "*.cpp")
file(GLOB_RECURSE SOURCE_FILES "../CaiqueNES/Core/*.cpp" "../CaiqueNES/Mappers/*.cpp" "../CaiqueNES/Utils/*.cpp")

# So that included headers can be found
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

add_executable(caique-nes-tests ${TEST_FILES} ${SOURCE_FILES})

# Define file paths for file-based tests
target_compile_definitions(caique-nes-tests PRIVATE
        "TESTING_ENVIRONMENT_NESTEST"
        "TESTING_ENVIRONMENT_6502"
        "TESTING_ENVIRONMENT_BLARGG"
        "TEST_LOG_FILE_NESTEST=\"${CMAKE_SOURCE_DIR}/Tests/External/NesTest/Logs/nestest.log\""
        "TEST_ROM_FILE_NESTEST=\"${CMAKE_SOURCE_DIR}/Tests/External/NesTest/ROM/nestest.nes\""
        "TEST_DIR_6502=\"${CMAKE_SOURCE_DIR}/Tests/External/6502/json/\""
        "TEST_DIR_BLARGG=\"${CMAKE_SOURCE_DIR}/Tests/External/Blargg/\"")

find_package(rapidjson REQUIRED)
include_directories("${RAPIDJSON_INCLUDE_DIRS}")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${RAPIDJSON_CXX_FLAGS}")

target_link_libraries(caique-nes-tests gtest_main)

target_include_directories(caique-nes-tests PRIVATE ${CMAKE_SOURCE_DIR}/CaiqueNES)

include(GoogleTest)
gtest_discover_tests(caique-nes-tests)
set_target_properties(caique-nes-tests PROPERTIES CXX_STANDARD 23)
