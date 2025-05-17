# tests/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

message(STATUS "Current source dir: ${CMAKE_CURRENT_SOURCE_DIR}")
file(GLOB TEST_SRC *.cc)
message(STATUS "TEST_SRC = ${TEST_SRC}")

list(LENGTH TEST_SRC NUM_SOURCES)
if(NUM_SOURCES EQUAL 0)
  message(WARNING "No test source files found!  (tests/*.cc, *.cpp)")
else()
  enable_testing()
  add_executable(runTests ${TEST_SRC})
  target_link_libraries(runTests PRIVATE mapping_lib gtest gtest_main pthread)

  include(GoogleTest)
  # add_test(NAME test_lbnPool COMMAND runTests)
  add_test(NAME AllTests COMMAND runTests)
endif()
