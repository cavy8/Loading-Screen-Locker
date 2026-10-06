cmake_minimum_required(VERSION 3.21)

# ---- Project Configuration ----
set(PROJECT_NAME "LoadingScreenLocker")
set(PROJECT_VERSION 1.0.0)

# CommonLib NG CMake setup
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# ---- Project ----
project(
    ${PROJECT_NAME}
    VERSION ${PROJECT_VERSION}
    LANGUAGES CXX
)

# ---- Include guards ----
if(PROJECT_SOURCE_DIR STREQUAL PROJECT_BINARY_DIR)
    message(FATAL_ERROR "In-source builds are not allowed.")
endif()

# ---- Dependencies ----
include(FetchContent)

# Build one DLL for SE (1.5), AE (1.6/1.7), and VR.
set(ENABLE_SKYRIM_SE ON CACHE BOOL "Support Skyrim SE" FORCE)
set(ENABLE_SKYRIM_AE ON CACHE BOOL "Support Skyrim AE" FORCE)
set(ENABLE_SKYRIM_VR ON CACHE BOOL "Support Skyrim VR" FORCE)
set(BUILD_TESTS OFF CACHE BOOL "Build CommonLibSSE tests")
# This plugin only writes a vtable entry; it does not use trampoline patches.
set(SKSE_SUPPORT_PATCH_SAFETY OFF CACHE BOOL "Validate trampoline patch sites")

FetchContent_Declare(
    CommonLibSSE
    GIT_REPOSITORY https://github.com/alandtse/CommonLibSSE-NG.git
    GIT_TAG 94faaed0c60eddd8347767f2d4d29a97c93bde8c # 11.0.0, ng
    GIT_SUBMODULES extern/openvr
)
FetchContent_MakeAvailable(CommonLibSSE)
include(${commonlibsse_SOURCE_DIR}/cmake/CommonLibSSE.cmake)
find_path(SIMPLEINI_INCLUDE_DIRS SimpleIni.h REQUIRED)

option(LSL_BUILD_TESTS "Build runtime compatibility checks" OFF)
if(LSL_BUILD_TESTS)
    enable_testing()
    add_executable(RuntimeCompatibilityTests tests/RuntimeCompatibility.cpp)
    target_link_libraries(RuntimeCompatibilityTests PRIVATE CommonLibSSE::CommonLibSSE)
    target_compile_definitions(RuntimeCompatibilityTests PRIVATE ENABLE_COMMONLIBSSE_TESTING=1)
    if(MSVC)
        target_compile_options(RuntimeCompatibilityTests PRIVATE /EHsc /utf-8)
    endif()
    add_test(NAME RuntimeCompatibility COMMAND RuntimeCompatibilityTests
        "${commonlibsse_SOURCE_DIR}/tests/REL")
endif()

# ---- Source Files ----
set(SOURCES
    src/main.cpp
    src/InputHandler.cpp
    src/Settings.cpp
)

set(HEADERS
    src/PCH.h
    src/InputHandler.h
    src/Settings.h
)

# ---- Create DLL ----
add_commonlibsse_plugin(
    ${PROJECT_NAME}
    AUTHOR "cavy8"
    USE_ADDRESS_LIBRARY
    SOURCES ${SOURCES} ${HEADERS}
)

target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_23)

# ---- Use static runtime to match CommonLibSSE ----
set_property(TARGET ${PROJECT_NAME} PROPERTY
    MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>"
)

# ---- Include directories ----
target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/src
        ${SIMPLEINI_INCLUDE_DIRS}
)

# ---- Precompiled headers ----
target_precompile_headers(${PROJECT_NAME}
    PRIVATE
        src/PCH.h
)

target_link_libraries(${PROJECT_NAME} PUBLIC CommonLibSSE::CommonLibSSE)

# ---- Post-build: Copy to Skyrim mods folder (if environment variable is set) ----
if(DEFINED ENV{SKYRIM_MODS_FOLDER})
    set(MOD_FOLDER "$ENV{SKYRIM_MODS_FOLDER}/${PROJECT_NAME}")
    
    add_custom_command(
        TARGET ${PROJECT_NAME}
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${MOD_FOLDER}/SKSE/Plugins"
        COMMAND ${CMAKE_COMMAND} -E copy_if_different "$<TARGET_FILE:${PROJECT_NAME}>" "${MOD_FOLDER}/SKSE/Plugins/"
        COMMENT "Copying LoadingScreenLocker.dll to ${MOD_FOLDER}/SKSE/Plugins/"
        VERBATIM
    )
endif()

# ---- Compiler-specific settings ----
if(MSVC)
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            /W4         # Warning level 4
            /WX-        # Don't treat warnings as errors (for now)
            /permissive-
            /Zc:__cplusplus
            /Zc:preprocessor
            /EHsc
            /utf-8      # Required for fmt library Unicode support
    )
endif()
