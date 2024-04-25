# ---Choose CMake version---
cmake_minimum_required(VERSION 3.20)

# ---Choose project name---
project(Simple-p6-Setup)

# ---Declare source files---
file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS src/*)
add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# ---Choose C++ version---
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_20)

# ---Choose warning level---
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -pedantic-errors -Wimplicit-fallthrough)
endif()

# ---Maybe enable warnings as errors---
set(WARNINGS_AS_ERRORS OFF CACHE BOOL "ON iff you want to treat warnings as errors")

if(WARNINGS_AS_ERRORS)
    if(MSVC)
        target_compile_options(${PROJECT_NAME} PRIVATE /WX)
    else()
        target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
    endif()
endif()

# ---Setup Testing---
include(FetchContent)
FetchContent_Declare(
    doctest
    GIT_REPOSITORY https://github.com/doctest/doctest
    GIT_TAG ae7a13539fb71f270b87eb2e874fbac80bc8dda2
)
FetchContent_MakeAvailable(doctest)
target_link_libraries(${PROJECT_NAME} PRIVATE doctest::doctest)

# ---Add p6 library---
# set(P6_RAW_OPENGL_MODE ON CACHE BOOL "") # You need to press F1 and use "CMake: Delete Cache and Reconfigure" when enabling this option.
#FetchContent_Declare(
#    p6
#    GIT_REPOSITORY https://github.com/julesfouchy/p6
#    GIT_TAG c962f8c5f3bf8324482facaf2ec178ef4c9aefaa
#)
#FetchContent_MakeAvailable(p6)
#target_link_libraries(${PROJECT_NAME} PRIVATE p6::p6)

# ---Windowing---
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw
    GIT_TAG 76227fa5192e497d6c1417bb607d2f49e857061d
)
FetchContent_MakeAvailable(glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

# ---glm---
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm
    GIT_TAG 33b0eb9fa336ffd8551024b1d2690e418014553b
)
FetchContent_MakeAvailable(glm)
target_link_libraries(${PROJECT_NAME} PRIVATE glm)

# ---exe_path---
FetchContent_Declare(
    exe_path
    GIT_REPOSITORY https://github.com/CoolLibs/exe_path
    GIT_TAG 198dd0c2a077ffaed5ecbe9f45b9836e8b3c9fc7
)
FetchContent_MakeAvailable(exe_path)
target_link_libraries(${PROJECT_NAME} PRIVATE exe_path)

# ---Add glad---
add_library(glad third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

# ---Add tinyobjloader---
target_include_directories(${PROJECT_NAME} PRIVATE third_party/tinyobjloader)
target_include_directories(${PROJECT_NAME} PRIVATE third_party/stb_image)
