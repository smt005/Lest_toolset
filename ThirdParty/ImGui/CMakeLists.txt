cmake_minimum_required(VERSION 3.15) # Проверка версии CMake.

# Переменные
set(CMAKE_CXX_STANDARD 17)

# Расположение файлов библиотеки
#FILE(GLOB_RECURSE INCLUDES ${IMGUI_SOURCE}/*.h)
#FILE(GLOB_RECURSE SOURCES ${IMGUI_SOURCE}/*.cpp)
FILE(GLOB INCLUDES ${IMGUI_SOURCE}/*.h)
FILE(GLOB SOURCES ${IMGUI_SOURCE}/*.cpp)

list(APPEND INCLUDES "${IMGUI_SOURCE}/backends/imgui_impl_glfw.h")
list(APPEND INCLUDES "${IMGUI_SOURCE}/backends/imgui_impl_opengl3.h")
list(APPEND SOURCES "${IMGUI_SOURCE}/backends/imgui_impl_glfw.cpp")
list(APPEND SOURCES "${IMGUI_SOURCE}/backends/imgui_impl_opengl3.cpp")

message("SOURCE: " ${INCLUDES} ${SOURCES})

source_group(TREE ${IMGUI_SOURCE} TREE PREFIX "" FILES ${INCLUDES} ${SOURCES})

# Создание статической библиотеки
add_library(${IMGUI_NAME} STATIC ${INCLUDES} ${SOURCES})
