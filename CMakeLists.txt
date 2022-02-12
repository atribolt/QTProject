cmake_minimum_required(VERSION "3.10")

project(DvplTool
  VERSION "1.0.0"
)

set(LIB_HEADERS ${PROJECT_SOURCE_DIR}/include)

add_subdirectory(libs)
add_subdirectory(UnpackDvpl)
