# tests/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ▶︎ 1. 自動追蹤檔案變動
file(GLOB CONFIGURE_DEPENDS TEST_SRC test_lbn *.cc *.cpp)
message(STATUS "TEST_SRC = ${TEST_SRC}")

list(LENGTH TEST_SRC NUM_SOURCES)
if(NUM_SOURCES EQUAL 0)
  message(WARNING "No test source files found!  (tests/*.cc, *.cpp)")
else()
  add_executable(runTests ${TEST_SRC})
  target_link_libraries(runTests PRIVATE mapping_lib gtest gtest_main pthread)

  include(GoogleTest)
  gtest_discover_tests(runTests)
endif()
