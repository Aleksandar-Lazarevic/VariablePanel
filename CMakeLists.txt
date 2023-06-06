cmake_minimum_required(VERSION 3.23)


# set the project name
project(EditorCore)

if(MSVC)
add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
# add _UNICODE UNICODE compile definitions if you want unicode windows functions
endif()

#set(CMAKE_DEBUG_POSTFIX "d")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
option(BUILD_SHARED_LIBS "Are the libraries DLLs" NO)
option(SKIP_INSTALL_ALL "dont do the installers" YES)

add_compile_definitions(_CRT_NONSTDC_NO_WARNINGS) 

cmake_policy(SET CMP0074 NEW)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(EditorFrame)
add_subdirectory(SimpleHazel/ForcePanel)
add_subdirectory(SimpleHazel/DockingRangeSelectImGUI)
add_subdirectory(SimpleHazel/Hazel)

set_property(GLOBAL PROPERTY VS_STARTUP_PROJECT EditorFrame)