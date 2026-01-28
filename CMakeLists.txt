cmake_minimum_required(VERSION 3.18...3.27)
project(lillaspel VERSION 0.0.1 LANGUAGES CXX)

# Use a modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define unicode macros consistently
add_compile_definitions(UNICODE _UNICODE)

# Where to put built artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

include(FetchContent)

# Directory for pre-built libraries
set(LIBRARY_DIR "${CMAKE_BINARY_DIR}/lib")
file(MAKE_DIRECTORY "${LIBRARY_DIR}")

# Download nlohmann::json single-header
set(JSON_SINGLE_INCLUDE "${CMAKE_BINARY_DIR}/include/nlohmann/json.hpp")
if(NOT EXISTS "${JSON_SINGLE_INCLUDE}")
  file(MAKE_DIRECTORY "${CMAKE_BINARY_DIR}/include")
  message(STATUS "Downloading nlohmann/json single header to ${JSON_SINGLE_INCLUDE}")
  file(DOWNLOAD
    "https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp"
    "${JSON_SINGLE_INCLUDE}"
    TLS_VERIFY ON
    SHOW_PROGRESS
  )
else()
  message(STATUS "Using existing nlohmann/json header at ${JSON_SINGLE_INCLUDE}")
endif()

# Fetch DirectXTK
FetchContent_Declare(
  DirectXTK
  GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
  GIT_TAG oct2025
)
FetchContent_MakeAvailable(DirectXTK)

# Fetch OpenAL
FetchContent_Declare(
  OpenAL
  GIT_REPOSITORY https://github.com/kcat/openal-soft
  GIT_TAG 1.25.1
)
FetchContent_MakeAvailable(OpenAL)

# Download pre-built libogg
set(LIBOGG_DLL "${LIBRARY_DIR}/libogg.dll")
set(LIBOGG_LIB "${LIBRARY_DIR}/libogg.lib")
if(NOT EXISTS "${LIBOGG_DLL}")
  message(STATUS "Downloading libogg.dll")
  file(DOWNLOAD
    "https://github.com/xiph/ogg/releases/download/v1.3.5/libogg-1.3.5-windows-x64-bin.zip"
    "${LIBRARY_DIR}/libogg.zip"
    TLS_VERIFY ON
  )
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${LIBRARY_DIR}/libogg.zip"
    WORKING_DIRECTORY "${LIBRARY_DIR}"
  )
  file(GLOB LIBOGG_FILES "${LIBRARY_DIR}/*.dll" "${LIBRARY_DIR}/*.lib")
  file(COPY ${LIBOGG_FILES} DESTINATION "${LIBRARY_DIR}")
endif()

# Download pre-built libvorbis
set(LIBVORBIS_DLL "${LIBRARY_DIR}/libvorbis.dll")
set(LIBVORBIS_LIB "${LIBRARY_DIR}/libvorbis.lib")
set(LIBVORBISFILE_DLL "${LIBRARY_DIR}/libvorbisfile.dll")
set(LIBVORBISFILE_LIB "${LIBRARY_DIR}/libvorbisfile.lib")
if(NOT EXISTS "${LIBVORBIS_DLL}")
  message(STATUS "Downloading libvorbis.dll and libvorbisfile.dll")
  file(DOWNLOAD
    "https://github.com/xiph/vorbis/releases/download/v1.3.7/libvorbis-1.3.7-windows-x64-bin.zip"
    "${LIBRARY_DIR}/libvorbis.zip"
    TLS_VERIFY ON
  )
  execute_process(COMMAND ${CMAKE_COMMAND} -E tar xzf "${LIBRARY_DIR}/libvorbis.zip"
    WORKING_DIRECTORY "${LIBRARY_DIR}"
  )
  file(GLOB LIBVORBIS_FILES "${LIBRARY_DIR}/*.dll" "${LIBRARY_DIR}/*.lib")
  file(COPY ${LIBVORBIS_FILES} DESTINATION "${LIBRARY_DIR}")
endif()

# ===============================
# libsndfile (binary + headers)
# ===============================

set(LIBSNDFILE_VERSION 1.2.2)

set(LIBSNDFILE_ROOT        "${LIBRARY_DIR}/libsndfile")
set(LIBSNDFILE_DLL         "${LIBSNDFILE_ROOT}/bin/libsndfile.dll")
set(LIBSNDFILE_LIB         "${LIBSNDFILE_ROOT}/lib/libsndfile-.lib")
set(LIBSNDFILE_INCLUDE_DIR "${LIBSNDFILE_ROOT}/include")

if(NOT EXISTS "${LIBSNDFILE_DLL}")
  message(STATUS "Downloading libsndfile ${LIBSNDFILE_VERSION}")

  file(DOWNLOAD
    "https://github.com/libsndfile/libsndfile/releases/download/${LIBSNDFILE_VERSION}/libsndfile-${LIBSNDFILE_VERSION}-windows-x64-bin.zip"
    "${LIBRARY_DIR}/libsndfile-bin.zip"
    TLS_VERIFY ON
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${LIBRARY_DIR}/libsndfile-bin.zip"
    WORKING_DIRECTORY "${LIBRARY_DIR}"
  )

  file(MAKE_DIRECTORY
    "${LIBSNDFILE_ROOT}/bin"
    "${LIBSNDFILE_ROOT}/lib"
    "${LIBSNDFILE_ROOT}/include"
  )

  file(GLOB _SNDFILE_DLL "${LIBRARY_DIR}/*.dll")
  file(GLOB _SNDFILE_LIB "${LIBRARY_DIR}/*.lib")

  file(COPY ${_SNDFILE_DLL} DESTINATION "${LIBSNDFILE_ROOT}/bin")
  file(COPY ${_SNDFILE_LIB} DESTINATION "${LIBSNDFILE_ROOT}/lib")

  message(STATUS "Downloading libsndfile headers")

  file(DOWNLOAD
    "https://github.com/libsndfile/libsndfile/archive/refs/tags/${LIBSNDFILE_VERSION}.zip"
    "${LIBRARY_DIR}/libsndfile-src.zip"
    TLS_VERIFY ON
  )

  execute_process(
    COMMAND ${CMAKE_COMMAND} -E tar xzf "${LIBRARY_DIR}/libsndfile-src.zip"
    WORKING_DIRECTORY "${LIBRARY_DIR}"
  )

  # Copy *contents* of include/, not the directory itself
  file(COPY
    "${LIBRARY_DIR}/libsndfile-${LIBSNDFILE_VERSION}/include/"
    DESTINATION "${LIBSNDFILE_INCLUDE_DIR}"
  )
endif()


# Fetch fastgltf
FetchContent_Declare(
  fastgltf
  GIT_REPOSITORY https://github.com/spnda/fastgltf/
  GIT_TAG v0.9.0
)
FetchContent_MakeAvailable(fastgltf)

# Fetch ImGui
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.82
)
FetchContent_GetProperties(imgui)
if(NOT imgui_POPULATED)
  FetchContent_MakeAvailable(imgui)
endif()

# Collect all project sources
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h" "${CMAKE_SOURCE_DIR}/headers/*.hpp")

add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Include project headers and generated/include directory
target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_SOURCE_DIR}/headers"
  "${CMAKE_BINARY_DIR}/include"
  "${LIBSNDFILE_INCLUDE_DIR}"
)

# Add ImGui sources if populated
if(imgui_SOURCE_DIR)
  target_sources(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
  )
  target_include_directories(${PROJECT_NAME} PRIVATE
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
  )
endif()

# Link libraries
target_link_libraries(${PROJECT_NAME} PRIVATE
  d3d11
  DirectXTK
  OpenAL::OpenAL
  "${LIBSNDFILE_LIB}"
  fastgltf
  "${LIBOGG_LIB}"
  "${LIBVORBIS_LIB}"
  "${LIBVORBISFILE_LIB}"
)

# Copy DLLs to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
  COMMAND ${CMAKE_COMMAND} -E copy
    "${LIBOGG_DLL}"
    "${LIBVORBIS_DLL}"
    "${LIBVORBISFILE_DLL}"
    "${LIBSNDFILE_DLL}"
    "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
)

# Ensure consistent output directories for the target
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

message(STATUS "Configured ${PROJECT_NAME} with ${CMAKE_CXX_STANDARD}-standard and ${PROJECT_SOURCES} source files")
