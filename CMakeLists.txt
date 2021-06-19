cmake_minimum_required(VERSION 3.11)

project("GSoC")

 

# Maybe stop from CMAKEing in the wrong place
if (CMAKE_BINARY_DIR STREQUAL CMAKE_SOURCE_DIR)
message(FATAL_ERROR "Source and build directories cannot be the same. Go use the /build directory.")
endif()

### Configure output locations
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

### Compiler options
set( CMAKE_EXPORT_COMPILE_COMMANDS 1 ) # Emit a compile flags file to support completion engines

# Add polyscope
message("\n\n == CMAKE recursively building Polyscope\n")
add_subdirectory("deps/polyscope")
# Add ponca
message("\n\n == CMAKE recursively building Polyscope\n")
add_subdirectory("deps/ponca")

# Add geometry-central
message("\n\n == CMAKE recursively building geometry-central\n")
add_subdirectory("deps/geometry-central")
  # Create an executable
  add_executable(
    example
    src/main.cpp
    )

set_property(TARGET example PROPERTY CXX_STANDARD 17)

# Link settings
target_link_libraries(example polyscope Ponca::Fitting geometry-central)
