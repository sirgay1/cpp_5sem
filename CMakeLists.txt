cmake_minimum_required(VERSION 3.15)

project(CalculatorApp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ------------------------------
# ��������� ������������
# ------------------------------
add_executable(calc
    src/main.cpp
    src/calculator.cpp
    src/plugin_manager.cpp
)

target_include_directories(calc PRIVATE src)

# ------------------------------
# ����� ��� DLL-��������
# ------------------------------
set(PLUGIN_OUTPUT_DIR ${CMAKE_BINARY_DIR}/plugins)
file(MAKE_DIRECTORY ${PLUGIN_OUTPUT_DIR})

# ------------------------------
# ������ ��� ������ ������ �������
# ------------------------------
function(add_calc_plugin NAME SRCFILE)
    add_library(${NAME} SHARED ${SRCFILE})
    set_target_properties(${NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
        LIBRARY_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
        ARCHIVE_OUTPUT_DIRECTORY ${PLUGIN_OUTPUT_DIR}
    )
endfunction()

# ------------------------------
# ������� (������ DLL)
# ------------------------------
add_calc_plugin(funcsin   plugins_src/funcsin.cpp)
add_calc_plugin(funcln    plugins_src/funcln.cpp)

# ------------------------------
# ��������� ����� ������
# ------------------------------
message(STATUS "Plugins will be placed in: ${PLUGIN_OUTPUT_DIR}")
