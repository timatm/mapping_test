# tests/CMakeLists.txt
include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# ▶︎ 1. 自動追蹤檔案變動
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
  gtest_discover_tests(runTests           # 目標名
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}  # 執行目錄
    DISCOVERY_TIMEOUT  30                 # 秒；預設 5
    TEST_PREFIX      "Unit."              # 自訂前綴，可省略
    TEST_SUFFIX      ""                   # 亦可自訂後綴
    EXTRA_ARGS       --myflag=1           # 需要額外參數時
  )
endif()
