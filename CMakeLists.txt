cmake_minimum_required(VERSION 3.16)
project(miniRenderer LANGUAGES C CXX)

# --- 1. GLOBAL SETTINGS ---
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --- 2. APPLE SILICON & MAC SPECIFICS ---
if(APPLE)
    set(CMAKE_PREFIX_PATH "/opt/homebrew")
    set(CMAKE_OSX_ARCHITECTURES "arm64")
    add_definitions(-DGL_SILENCE_DEPRECATION)
    set(CMAKE_MACOSX_RPATH 1)
endif()

# --- 3. FIND SYSTEM DEPENDENCIES ---
find_package(glfw3 REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

# --- 4. INCLUDE DIRECTORIES ---
include_directories(
    include
    external/include
    external/imgui
    external/imgui/backends
)

# --- 5. COLLECT SOURCES ---
file(GLOB SOURCES
   "src/*.cpp"
    "src/*.c"
    "external/src/*.c"
    "external/imgui/*.cpp"
)

list(APPEND SOURCES
    "external/imgui/backends/imgui_impl_glfw.cpp"
    "external/imgui/backends/imgui_impl_opengl3.cpp"
)
# --- 6. THE EXECUTABLE ---
add_executable(${PROJECT_NAME} ${SOURCES})

# --- 7. LINK LIBRARIES ---
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
