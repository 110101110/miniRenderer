cmake_minimum_required(VERSION 3.16)
project(miniRenderer LANGUAGES C CXX)

# --- 1. GLOBAL SETTINGS ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON) # Useful for VSCode Intellisense

# --- 2. MAC SPECIFICS & HOMEBREW ---
if(APPLE)
    # Tell CMake where Homebrew lives on Apple Silicon
    set(CMAKE_PREFIX_PATH "/opt/homebrew")

    # Silence Apple's "OpenGL is deprecated" warnings
    add_definitions(-DGL_SILENCE_DEPRECATION)

    # Ensure RPATH is set so the app finds .dylib files
    set(CMAKE_MACOSX_RPATH 1)
endif()

# --- 3. FIND SYSTEM DEPENDENCIES (BREW) ---
find_package(glfw3 REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# --- 4. INCLUDE DIRECTORIES ---
# This tells the compiler where to look for .h and .hpp files
include_directories(
    include
    external/include
)

# --- 5. COLLECT SOURCE FILES ---
# We use recursive globbing to find all .cpp and .c (for glad)
file(GLOB_RECURSE SOURCES
    "src/main.cpp"
    # "src/*.c"
    "external/src/*.c"
    # "external/imgui/*.cpp"
)

# --- 6. THE EXECUTABLE ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- 7. LINK LIBRARIES ---
# This "stitches" the libraries into your final binary
target_link_libraries(${PROJECT_NAME} PRIVATE
    glfw
    glm::glm
    ${OPENGL_LIBRARIES}
)


if(APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE
        "-framework Cocoa"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework CoreFoundation"
    )
endif()

file(COPY shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
