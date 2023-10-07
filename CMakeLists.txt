cmake_minimum_required(VERSION 3.15) # Проверка версии CMake.

# Проект
set(PROJECT_DIR ${CMAKE_CURRENT_SOURCE_DIR})
set(PROJECT_NAME LestaTools)
set(PROJECT_DIR_FILES "${PROJECT_DIR}")

FILE(GLOB_RECURSE PROJECT_INCLUDE ${PROJECT_DIR_FILES}/Source/*.h)
FILE(GLOB_RECURSE PROJECT_SOURCE ${PROJECT_DIR_FILES}/Source/*.cpp)
source_group(TREE ${PROJECT_DIR_FILES} TREE PREFIX "" FILES ${PROJECT_INCLUDE} ${PROJECT_SOURCE})

#
set(CMAKE_CXX_STANDARD 17)

# Библиотеки
# jsoncpp
set(JSONCPP_NAME jsoncpp)
set(JSONCPP_DIR "${PROJECT_DIR_FILES}/ThirdParty/jsoncpp")
set(JSONCPP_INCLUDES "${JSONCPP_DIR}/include")
set(JSONCPP_SOURCE "${JSONCPP_DIR}/src")
set(JSONCPP_LIB "${PROJECT_DIR}/../build/Win/Engine/ThirdParty/jsoncpp/src/lib_json")

FILE(GLOB_RECURSE JSONCPP_ALL_INCLUDES ${JSONCPP_INCLUDES}/*.h)
FILE(GLOB_RECURSE JSONCPP_ALL_SOURCES ${JSONCPP_SOURCE}/*.cpp)

# GLFW
set(GLFW_BUILD_EXAMPLES 0)
set(GLFW_BUILD_TESTS 0)
set(DOXYGEN_FOUND 0)
set(GLFW_BUILD_DOCS 0)

set(GLFW_NAME glfw)
set(GLFW_DIR "${PROJECT_DIR_FILES}/ThirdParty/glfw")
set(GLFW_INCLUDES "${GLFW_DIR}/include")
set(GLFW_SOURCE "${GLFW_DIR}/src")
set(GLFW_LIB "${PROJECT_DIR}/../build/Win/Engine/ThirdParty/glfw/src")

FILE(GLOB_RECURSE GLFW_ALL_INCLUDES ${GLFW_INCLUDES}/*.h)
FILE(GLOB_RECURSE GLFW_ALL_SOURCES ${GLFW_SOURCE}/*.cpp)

# ImGui
set(IMGUI_NAME ImGui)
set(IMGUI_DIR "${PROJECT_DIR_FILES}/ThirdParty/ImGui")
set(IMGUI_INCLUDES "${IMGUI_DIR}")
set(IMGUI_SOURCE "${IMGUI_DIR}")

# FreeImage
set(FREEIMAGE_NAME FreeImage)
set(FREEIMAGE_DIR "${PROJECT_DIR_FILES}/ThirdParty/FreeImage")
set(FREEIMAGE_LIB "${FREEIMAGE_DIR}/lib")
set(FREEIMAGE_INCLUDES "${FREEIMAGE_DIR}/src")
set(FREEIMAGE_DLL "${FREEIMAGE_DIR}/dll")

# Создание
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_DIR}/Build/Libs)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_DIR}/Build/Libs)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_DIR}/Tools)

project(${PROJECT_NAME})
add_executable(${PROJECT_NAME} ${PROJECT_INCLUDE} ${PROJECT_SOURCE})
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
set_property(TARGET ${PROJECT_NAME}  PROPERTY VS_DEBUGGER_WORKING_DIRECTORY ${PROJECT_DIR}/Tools)

# Библиотеки
# jsoncpp
include_directories(${JSONCPP_INCLUDES})
link_directories(${JSONCPP_LIB})
add_library(${JSONCPP_NAME} STATIC ${JSONCPP_ALL_SOURCES})
add_subdirectory(${JSONCPP_SOURCE})
target_link_libraries(${PROJECT_NAME} ${JSONCPP_NAME})

# glFw
include_directories(${GLFW_INCLUDES})
link_directories(${GLFW_LIB})
add_subdirectory(${GLFW_DIR})
target_link_libraries(${PROJECT_NAME} ${GLFW_NAME})

# ImGui
include_directories(${IMGUI_INCLUDES})
add_subdirectory(${IMGUI_DIR})
target_link_libraries(${PROJECT_NAME} ${IMGUI_NAME})
link_directories(${IMGUI_DIR})

# FreeImage
#include_directories(${FREEIMAGE_INCLUDES})
#link_directories(${FREEIMAGE_LIB})
target_link_libraries(${PROJECT_NAME} "${FREEIMAGE_LIB}/${FREEIMAGE_NAME}.lib")
file(COPY "${FREEIMAGE_DLL}/${FREEIMAGE_NAME}.dll" DESTINATION ${PROJECT_DIR}/Tools/debug)
file(COPY "${FREEIMAGE_DLL}/${FREEIMAGE_NAME}.dll" DESTINATION ${PROJECT_DIR}/Tools/release)

# version
add_custom_target( git_version ALL
    "${CMAKE_COMMAND}" -E chdir "${CMAKE_CURRENT_SOURCE_DIR}" "${CMAKE_COMMAND}" -P "Source/Extra/version.cmake"
)
add_dependencies( ${PROJECT_NAME} git_version )
