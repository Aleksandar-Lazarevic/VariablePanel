cmake_minimum_required(VERSION 3.23)

# set the project name
project(ForcePanel)

set(
    source_list
	"ForcePanel/ForcePanel.cpp"
	"ForcePanel/ForcePanel.hpp"
	"ForcePanel/ForcePanel_exports.hpp"
	"Widgets/imgui_widgets.h"
	"Widgets/imgui_widgets.cpp"
	"Widgets/imgui_scopes.hpp"
	"Widgets/imgui_utils.h"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
			#PREFIX "Source Files"
			FILES ${source_list})

add_library(${PROJECT_NAME} ${source_list})

target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(${PROJECT_NAME} DockingRangeSelectImGUI Hazel)

if(BUILD_SHARED_LIBS)
	target_compile_definitions(${PROJECT_NAME} INTERFACE "${PROJECT_NAME}_IMPORTS")
endif()

if(MSVC)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
