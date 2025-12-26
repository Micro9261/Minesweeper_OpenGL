cmake_minimum_required(VERSION 4.0.0)
project(Minesweeper_OpenGL CXX)
cmake_policy(SET CMP0169 OLD)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG_INIT "-Wall")
set(CMAKE_CXX_FLAGS_RELEASE_NIT "-Wall")

include(FetchContent)

# --------------------------------------------------------------------------
# Save all static libs to Lib/
# --------------------------------------------------------------------------
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/Lib)

#============================================================================
# SFML 3.0.2 (STATIC)
#============================================================================
set(SFML_STATIC_LIBRARIES ON)
set(BUILD_SHARED_LIBS OFF)
set(SFML_BUILD_EXAMPLES OFF)
set(SFML_BUILD_TESTS OFF)
set(SFML_BUILD_DOC OFF)

FetchContent_Declare(
  SFML
  GIT_REPOSITORY https://github.com/SFML/SFML.git
  GIT_TAG 2.6.1
)
FetchContent_MakeAvailable(SFML)

# Required for static SFML
add_compile_definitions(SFML_STATIC)

#============================================================================
# ImGui (STATIC)
#============================================================================
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.91.1
)
FetchContent_MakeAvailable(imgui)

file(GLOB IMGUI_SRC
  ${imgui_SOURCE_DIR}/*.cpp
)

add_library(imgui STATIC ${IMGUI_SRC})
target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

#============================================================================
# ImGui-SFML (STATIC) — manual build, bypass CMakeLists.txt
#============================================================================
FetchContent_Declare(
  imgui_sfml
  GIT_REPOSITORY https://github.com/SFML/imgui-sfml.git
  GIT_TAG v2.6.1
)
FetchContent_Populate(imgui_sfml)

# Build ImGui-SFML manually
add_library(imgui-sfml STATIC
  ${imgui_sfml_SOURCE_DIR}/imgui-SFML.cpp
)

target_include_directories(imgui-sfml PUBLIC
  ${imgui_sfml_SOURCE_DIR}
  ${imgui_SOURCE_DIR}
  ${SFML_SOURCE_DIR}/include
)

target_link_libraries(imgui-sfml PUBLIC
  imgui
  sfml-graphics
  sfml-window
  sfml-system
)

# ==========================
# Fetch FreeType (static)
# ==========================
set(FT_DISABLE_ZLIB ON CACHE BOOL "" FORCE)
set(FT_DISABLE_BZIP2 ON CACHE BOOL "" FORCE)
set(FT_DISABLE_PNG ON CACHE BOOL "" FORCE)
set(FT_DISABLE_HARFBUZZ ON CACHE BOOL "" FORCE)

FetchContent_Declare(
    freetype
    GIT_REPOSITORY https://gitlab.freedesktop.org/freetype/freetype.git
    GIT_TAG VER-2-13-2   # stable version (recommended)
)

FetchContent_MakeAvailable(freetype)

#============================================================================
# OpenGL Configuration
#============================================================================

find_package(GLUT REQUIRED)
# find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

#============================================================================
# Doxygen documentation
#============================================================================
find_package(Doxygen)
if (NOT DOXYGEN_FOUND)
  message(WARNING "Doxygen not found, 'doc' target disabled")
  return()
endif()

if (DOXYGEN_FOUND)
  set(DOXYGEN_OUTPUT_DIR ${CMAKE_SOURCE_DIR}/Doc)
  set(DOXYGEN_INPUT_DIR
      ${CMAKE_SOURCE_DIR}/Oscillator/include
      ${CMAKE_SOURCE_DIR}/Oscillator/src
  )

  string(REPLACE ";" " " DOXYGEN_INPUT_DIR "${DOXYGEN_INPUT_DIR}")

  # Doxyfile template (generated)
  set(DOXYFILE_IN ${CMAKE_SOURCE_DIR}/Doxyfile.in)
  set(DOXYFILE_OUT ${CMAKE_BINARY_DIR}/Doxyfile)

  configure_file(${DOXYFILE_IN} ${DOXYFILE_OUT} @ONLY)

  add_custom_target(doc
      COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYFILE_OUT}
      WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
      COMMENT "Generating Doxygen documentation"
      VERBATIM
  )
endif()

#Project configuration
set(PROJECT_INCLUDES ${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_INCLUDES})

file(GLOB PROJECT_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
add_executable(${PROJECT_NAME} Main.cpp ${PROJECT_SOURCES})

target_link_libraries(${PROJECT_NAME}
  PRIVATE
  sfml-graphics
  sfml-window
  sfml-system
  imgui
  imgui-sfml
  opengl32
  glu32
  freetype
)

#============================================================================
# Copy assets to build dir
#============================================================================

set(ASSETS_DIR ${CMAKE_SOURCE_DIR}/assets)

add_custom_command(
  TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${ASSETS_DIR}
          $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
)

#============================================================================
# Tests googletest
#============================================================================

include( FetchContent)
FetchCOntent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)

set( gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(GameBoardTest tests/GameBoardTest.cpp ${PROJECT_SOURCES})
target_link_libraries(
  GameBoardTest
  GTest::gtest_main
  sfml-graphics
  sfml-window
  sfml-system
  imgui
  imgui-sfml
  opengl32
  glu32
  freetype
)

include(GoogleTest)
gtest_discover_tests(GameBoardTest)