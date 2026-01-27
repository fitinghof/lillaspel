#================================================
# CMAKE BUILD
# WINDOW BUILD CURRENTLY
#================================================
cmake_minimum_required(VERSION 3.20) # FetchContent is available in 3.11+
# https://www.programmersought.com/article/68945253024/

project(lillaspel VERSION 0.0.1)
message("START OF CMAKELIST...")

# https://bewagner.net/programming/2020/05/02/cmake-fetchcontent/
#set(FETCHCONTENT_FULLY_DISCONNECTED ON) # When this option is enabled, no attempt is made to download or update any content.
#set(FETCHCONTENT_UPDATES_DISCONNECTED ON)# 

# config IDE ext 
#include(CTest)
#enable_testing()

# headers and paths
set(CPP_INCLUDE_DIRS "./headers")
set(CPP_SOURCE_FILES "./src")
set(CPP_HEADER_FILES "./headers")

#================================================
# CONFIGS
#================================================
#set(HOST_OS "pc-Win32")
#set(CMAKE_BUILD_TYPE release32)
#set(USE32bit true)
#set(CMAKE_C_FLAGS -m32)
#set(CMAKE_CXX_FLAGS -m32)

# This section deal with the character sets UNICODE _UNICODE
# Directx win32 settings
# this deal with convert types
ADD_DEFINITIONS(-DUNICODE)
ADD_DEFINITIONS(-D_UNICODE)
#SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /UMBCS /D_UNICODE /DUNICODE")

# Directx win32 settings
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /SUBSYSTEM:CONSOLE")

set(CMAKE_C_STANDARD 11)
#set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

set(ENABLE_DXSDK OFF) # Directx SDK june 2010
set(ENABLE_WINDXSDK ON) # Windows Kits 10
set(WINDOWKIT_VERSION 10)
set(WINDOWKIT_VERSION_UPDATE 10.0.19041.0)
set(WINDOW_BIT x86) # arm, arm64, x64, x86

#testing...
set(ENABLE_DIRECTXHEADERS OFF)
set(ENABLE_DIRECTXTK12 OFF)
set(ENABLE_IMGUI ON)


include(FetchContent)

FetchContent_Declare(
    DirectXTK
    GIT_REPOSITORY https://github.com/microsoft/DirectXTK.git
    GIT_TAG main
)

FetchContent_MakeAvailable(DirectXTK)

FetchContent_Declare(
    OpenAL
    GIT_REPOSITORY https://github.com/kcat/openal-soft
    GIT_TAG 1.25.1
)
FetchContent_MakeAvailable(OpenAL)

# Download single header into your project
set(JSON_SINGLE_INCLUDE "${CMAKE_BINARY_DIR}/include/json.hpp")
file(DOWNLOAD
    "https://raw.githubusercontent.com/nlohmann/json/v3.11.3/single_include/nlohmann/json.hpp"
    ${JSON_SINGLE_INCLUDE}
    TLS_VERIFY ON
)

if(ENABLE_DXSDK)
  if( DEFINED ENV{DXSDK_DIR} ) # system path var
    set( DX_DIR $ENV{DXSDK_DIR})
  endif()

  list(APPEND CPP_INCLUDE_DIRS ${DX_DIR}/Include)

  link_directories(${DX_DIR}/Lib/${WINDOW_BIT})

endif()

if(ENABLE_WINDXSDK)

  message("WINDOW 10 PROGRAM LOCTION: " $ENV{ProgramFiles\(x86\)})
  set(PROGRAMFILES_PATH $ENV{ProgramFiles\(x86\)})
  set(WINDOW_KITS "${PROGRAMFILES_PATH}/Windows Kits/${WINDOWKIT_VERSION}")

  set(WINDOW_DXSDK_INCULDE "${WINDOW_KITS}/Include/${WINDOWKIT_VERSION}/um")

  set(WINDOW_DXSDK_LIB "${WINDOW_KITS}/Include/${WINDOWKIT_VERSION}/um/${WINDOW_BIT}")

  list(APPEND CPP_INCLUDE_DIRS ${WINDOW_DXSDK_INCULDE})
  link_directories(${WINDOW_DXSDK_LIB})
endif()

if(ENABLE_DIRECTXHEADERS)
  if (NOT directx_headers_FOUND) # If there's none, fetch and build
    include(FetchContent)
    FetchContent_Declare(
      directx_headers
      GIT_REPOSITORY https://github.com/microsoft/DirectX-Headers.git
      GIT_TAG v1.0.2 #main
    )
    FetchContent_GetProperties(directx_headers)
    if (NOT directx_headers_POPULATED) # Have we downloaded yet?
      set(FETCHCONTENT_QUIET NO)
      FetchContent_Populate(directx_headers)

      # include folder
      include_directories(${directx_headers_SOURCE_DIR}/include/directx)
      # build directx_headers
      add_subdirectory(${directx_headers_SOURCE_DIR} ${directx_headers_BINARY_DIR}) #add to build in sub dir
    endif()
  endif()
endif()

if(ENABLE_DIRECTXTK12)
  if (NOT directxtk12_FOUND) # If there's none, fetch and build
    include(FetchContent)
    FetchContent_Declare(
      directxtk12
      GIT_REPOSITORY https://github.com/microsoft/DirectXTK12
      GIT_TAG apr2021
    )
    FetchContent_GetProperties(directxtk12)
    if (NOT directxtk12_POPULATED) # Have we downloaded raylib yet?
      set(FETCHCONTENT_QUIET NO)
      FetchContent_Populate(directxtk12)
      add_subdirectory(${directxtk12_SOURCE_DIR} ${directxtk12_BINARY_DIR}) #add to build in sub dir
    endif()
  endif()
endif()

if(ENABLE_IMGUI)
  if (NOT imgui_FOUND) # If there's none, fetch and build imgui
    include(FetchContent)
    FetchContent_Declare(
      imgui
      GIT_REPOSITORY https://github.com/ocornut/imgui.git
      GIT_TAG v1.82
    )
    FetchContent_GetProperties(imgui)
    if (NOT imgui_POPULATED) # Have we downloaded yet?
      set(FETCHCONTENT_QUIET NO)
      FetchContent_Populate(imgui)
      # utility to setup the downloaded library for use
      #FetchContent_MakeAvailable(imgui)
      list(APPEND CPP_INCLUDE_DIRS ${imgui_SOURCE_DIR})
      list(APPEND CPP_INCLUDE_DIRS ${imgui_SOURCE_DIR}/backends)

      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/imgui.cpp)
      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/imgui_demo.cpp)
      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/imgui_draw.cpp)
      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/imgui_tables.cpp)
      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/imgui_widgets.cpp)

      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.cpp)
      list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.cpp)
      #list(APPEND CPP_SOURCE_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.cpp)

      list(APPEND CPP_HEADER_FILES ${imgui_SOURCE_DIR}/imgui.h)
      list(APPEND CPP_HEADER_FILES ${imgui_SOURCE_DIR}/imconfig.h)
      list(APPEND CPP_HEADER_FILES ${imgui_SOURCE_DIR}/imgui_internal.h)

      list(APPEND CPP_HEADER_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_win32.h)
      list(APPEND CPP_HEADER_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_dx11.h)
      #list(APPEND CPP_HEADER_FILES ${imgui_SOURCE_DIR}/backends/imgui_impl_dx12.h)

    endif()
  endif()
endif()


#================================================
# INCLUDE DIR FOLDERS
#================================================
include_directories(${CPP_INCLUDE_DIRS}) #include file folder
#================================================
# EXECUTE APP
#================================================
add_executable(${PROJECT_NAME} 
  WIN32 
  "src/main.cpp"
  ${CPP_HEADER_FILES}
  ${CPP_SOURCE_FILES}
)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_BINARY_DIR}/include")

# https://stackoverflow.com/questions/33137590/link-against-dx10-using-cmake
target_link_libraries(${PROJECT_NAME} 
  d3d11
  DirectXTK
  OpenAL::OpenAL
)

message("END OF CMAKELIST...")
