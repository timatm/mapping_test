file(GLOB SRC_FILES *.cc)

add_library(mapping_lib ${SRC_FILES})
target_include_directories(mapping_lib PUBLIC ${PROJECT_SOURCE_DIR}/include)
