cmake_minimum_required(VERSION 3.23)


# set the project name
project(EditorFrame)

set(
    source_list
	"EditorFrame/Editor.cpp"
	"EditorFrame/EditorData.h"
	"EditorFrame/Treeview/Node.hpp"
	"EditorFrame/Treeview/Node.cpp"
	"EditorFrame/Treeview/Listener.hpp"
	"EditorFrame/Treeview/Listener.cpp"
	"EditorFrame/Treeview/string_view_utils.cpp"
	"EditorFrame/Treeview/string_view_utils.hpp"
	"EditorFrame/Treeview/VariableValue.hpp"
	"EditorFrame/Treeview/VariableValue.cpp"
	"EditorFrame/Treeview/VariableNode.hpp"
	"EditorFrame/Treeview/VariableNode.cpp"
	"EditorFrame/Treeview/VariableFolder.hpp"
	"EditorFrame/Treeview/VariableFolder.cpp"
	"EditorFrame/TestEditorLayer.cpp"
	"EditorFrame/TestEditorLayer.h"
	"EditorFrame/TestScene.cpp"
	"EditorFrame/TestScene.h"
	"EditorFrame/EditorLayer.cpp"
	"EditorFrame/EditorLayer.h"
	"EditorFrame/FPSMonitor.cpp"
	"EditorFrame/FPSMonitor.hpp"
	"EditorFrame/Panels/VariablePanel.cpp"
	"EditorFrame/Panels/VariablePanel.hpp"
	"EditorFrame/Panels/TypeTesterPanel.cpp"
	"EditorFrame/Panels/TypeTesterPanel.h"
	"EditorFrame/Panels/SceneTesterPanel.cpp"
	"EditorFrame/Panels/SceneTesterPanel.h"
	"EditorFrame/Panels/ContentBrowserPanel.cpp"
	"EditorFrame/Panels/ContentBrowserPanel.h"
	"EditorFrame/Panels/SceneHierarchyPanel.cpp"
	"EditorFrame/Panels/SceneHierarchyPanel.h"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
			#PREFIX "Source Files"
			FILES ${source_list})

add_executable(${PROJECT_NAME} ${source_list})


find_package(glfw3 3.3.8 REQUIRED)
find_package(spdlog 1.10.0 REQUIRED)
#find_package(glew 2.2.0 REQUIRED)
find_package(RapidJSON REQUIRED)

message(STATUS "SPIRV-Tools library path: ${SPIRV_TOOLS_STATIC_LIBRARY}")

target_link_directories(${PROJECT_NAME} PRIVATE ${SPIRV_TOOLS_LIBRARY})

find_package(Boost 1.67.0)
if(Boost_FOUND)
  target_include_directories(${PROJECT_NAME} PUBLIC ${Boost_INCLUDE_DIRS})
else()
message(FATAL_ERROR "boost is required please use the cmake gui to set boost_dir")
endif()

target_link_libraries(${PROJECT_NAME} 
	#unofficial::shaderc::shaderc
	#ImGuiFrame 
	#glfw 
	Hazel 
	rapidjson
	#DockingRangeSelectImGUI 
	#spdlog::spdlog
	ForcePanel
	
	)

if(MSVC)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

function(copy_dll which_one)
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD 
	COMMAND ${CMAKE_COMMAND} -E copy_if_different 
	$<TARGET_FILE:${which_one}>
	$<TARGET_FILE_DIR:${PROJECT_NAME}>)
endfunction()

set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/working_dir")

if(BUILD_SHARED_LIBS)
copy_dll("ImGuiFrame")
copy_dll("ForcePanel")
endif()