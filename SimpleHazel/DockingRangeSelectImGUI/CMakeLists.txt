cmake_minimum_required(VERSION 3.23)


# set the project name
project(DockingRangeSelectImGUI)

set(
    source_list
	"imgui/imconfig.h"
	"imgui/imgui.cpp"
	"imgui/imgui.h"
	"imgui/imgui_demo.cpp"
	"imgui/imgui_draw.cpp"
	"imgui/imgui_internal.h"
	"imgui/imgui_tables.cpp"
	"imgui/imgui_widgets.cpp"
	"imgui/imstb_rectpack.h"
	"imgui/imstb_textedit.h"
	"imgui/imstb_truetype.h"
	"imgui/backends/imgui_impl_glfw.cpp"
	"imgui/backends/imgui_impl_glfw.h"
	"imgui/backends/imgui_impl_opengl3.cpp"
	"imgui/backends/imgui_impl_opengl3.h"
	"imgui/backends/imgui_impl_opengl3_loader.h"
	"imgui/misc/cpp/imgui_stdlib.cpp"
	"imgui/misc/cpp/imgui_stdlib.h"

	# imguizmo uses the api from imgui, and thus needs to be compiled as part of this library
	"imguizmo/GraphEditor.cpp"
"imguizmo/GraphEditor.h"
"imguizmo/ImCurveEdit.cpp"
"imguizmo/ImCurveEdit.h"
"imguizmo/ImGradient.cpp"
"imguizmo/ImGradient.h"
"imguizmo/ImGuizmo.cpp"
"imguizmo/ImGuizmo.h"
"imguizmo/ImSequencer.cpp"
"imguizmo/ImSequencer.h"
"imguizmo/ImZoomSlider.h"

)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
			#PREFIX "Source Files"
			FILES ${source_list})

add_library(${PROJECT_NAME} ${source_list})

find_package(glfw3 3.3.8 REQUIRED)

target_link_libraries(${PROJECT_NAME} PUBLIC glfw)

target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/imgui")
target_include_directories(${PROJECT_NAME} PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
if(BUILD_SHARED_LIBS)
	target_compile_definitions(${PROJECT_NAME} INTERFACE "${PROJECT_NAME}_IMPORTS")
endif()

if(MSVC)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
# add _UNICODE UNICODE compile definitions if you want unicode windows functions
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
