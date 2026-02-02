cmake_minimum_required(VERSION 3.18...3.27) # FetchContent is available in 3.11+
set(CMAKE_POLICY_VERSION_MINIMUM 3.5) # Makes sndfile happy

# https://www.programmersought.com/article/68945253024/

project(lillaspel VERSION 0.0.1 LANGUAGES CXX)
message(STATUS "Configuring ${PROJECT_NAME} v${PROJECT_VERSION}")

# Use a modern C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Define unicode macros consistently
add_compile_definitions(UNICODE _UNICODE)

# Where to put built artifacts
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")

include(FetchContent)

# Download nlohmann::json single-header into the build include dir so it can be used
# URL points to the 'develop' branch single_include header
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

# Fetch DirectXTK (provides a CMake target when available)
FetchContent_Declare(
  DirectXTK
  GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
  GIT_TAG oct2025
)
FetchContent_MakeAvailable(DirectXTK)

# Fetch OpenAL (openal-soft)
FetchContent_Declare(
  OpenAL
  GIT_REPOSITORY https://github.com/kcat/openal-soft
  GIT_TAG 1.25.1
)
FetchContent_MakeAvailable(OpenAL)

# Fetch sndfile
FetchContent_Declare(
  sndfile
  GIT_REPOSITORY https://github.com/libsndfile/libsndfile.git
  GIT_TAG 1.2.2
)
FetchContent_MakeAvailable(sndfile)

# fetch fastgltf
FetchContent_Declare(
  fastgltf
  GIT_REPOSITORY https://github.com/spnda/fastgltf/
  GIT_TAG v0.9.0
)
FetchContent_MakeAvailable(fastgltf)

# Optionally fetch ImGui for in-project use. We'll populate and add its sources to the target.
FetchContent_Declare(
  imgui
  GIT_REPOSITORY https://github.com/ocornut/imgui.git
  GIT_TAG v1.82
)
FetchContent_GetProperties(imgui)

if(NOT imgui_POPULATED)
  FetchContent_MakeAvailable(imgui)
endif()

# Collect all project sources (all .cpp files under src/)
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Optionally collect headers to show up in IDEs
file(GLOB_RECURSE PROJECT_HEADERS CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/headers/*.h" "${CMAKE_SOURCE_DIR}/headers/*.hpp")

add_executable(${PROJECT_NAME} WIN32 ${PROJECT_SOURCES} ${PROJECT_HEADERS})

# Include project headers and generated/include directory
target_include_directories(${PROJECT_NAME} PRIVATE
  "${CMAKE_SOURCE_DIR}/headers"
  "${CMAKE_BINARY_DIR}/include"
)

# If imgui was populated, add its sources and include directories.
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

# Link libraries. Keep d3d11 for DirectX usage and the fetched libraries.
# DirectXTK provides a target named "DirectXTK" in its CMake; OpenAL provides OpenAL::OpenAL.
target_link_libraries(${PROJECT_NAME} PRIVATE
  d3d11
  DirectXTK
  OpenAL::OpenAL
  sndfile
  fastgltf
)

# Ensure consistent output directories for the target
set_target_properties(${PROJECT_NAME} PROPERTIES
  RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
  ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}"
  LIBRARY_OUTPUT_DIRECTORY "${CMAKE_LIBRARY_OUTPUT_DIRECTORY}"
)

# Build HLSL shaders
set(SHADER_PATH "./src/shaders")
set(PS_SHADER "${SHADER_PATH}/pixelshaders/PSTest.hlsl")
set(PS_SHADER_UNLIT "${SHADER_PATH}/pixelshaders/psUnlit.hlsl")
set(PS_SHADER_LIT "${SHADER_PATH}/pixelshaders/psLit.hlsl")
set(VS_SHADER "${SHADER_PATH}/vertexshaders/VSTest.hlsl")
set(HLSL_SHADER_FILES ${PS_SHADER} ${PS_SHADER_UNLIT} ${PS_SHADER_LIT} ${VS_SHADER})

set_source_files_properties(${VS_SHADER} PROPERTIES ShaderType "vs")
set_source_files_properties(${PS_SHADER} PROPERTIES ShaderType "ps")
set_source_files_properties(${PS_SHADER_UNLIT} PROPERTIES ShaderType "ps")
set_source_files_properties(${PS_SHADER_LIT} PROPERTIES ShaderType "ps")
set_source_files_properties(${HLSL_SHADER_FILES} PROPERTIES ShaderModel "5_0")

foreach(FILE ${HLSL_SHADER_FILES})
  get_filename_component(FILE_WE ${FILE} NAME_WE)
  list(APPEND CSO_SHADER_FILES ${CMAKE_BINARY_DIR}/${FILE_WE}.cso)
  get_source_file_property(shadertype ${FILE} ShaderType)
  get_source_file_property(shadermodel ${FILE} ShaderModel)
  add_custom_command(TARGET ${PROJECT_NAME} PRE_LINK
                     COMMAND fxc.exe /nologo /Emain /T${shadertype}_${shadermodel} $<IF:$<CONFIG:DEBUG>,/Od,/O1> /Zi /Fo ${CMAKE_BINARY_DIR}/${FILE_WE}.cso /Fd ${CMAKE_BINARY_DIR}/${FILE_WE}.pdb ${FILE}
                     COMMENT "HLSL ${FILE}"
                     WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
                     VERBATIM)
endforeach(FILE)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
                  COMMAND ${CMAKE_COMMAND} -E copy ${CSO_SHADER_FILES} $<TARGET_FILE_DIR:${PROJECT_NAME}>
                  COMMAND_EXPAND_LISTS)

message(STATUS "Configured ${PROJECT_NAME} with ${CMAKE_CXX_STANDARD}-standard and ${PROJECT_SOURCES} source files")
