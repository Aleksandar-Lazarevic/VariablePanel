cmake_minimum_required(VERSION 3.23)

# set the project name
project(Hazel)

set(source_list
"src/Hazel.h"
"src/Hazel_exports.h"
"src/hzpch.h"
"src/Hazel/Core/Application.cpp"
"src/Hazel/Core/Application.h"
"src/Hazel/Core/Assert.h"
"src/Hazel/Core/Base.h"
"src/Hazel/Core/Buffer.h"
"src/Hazel/Core/EntryPoint.h"
"src/Hazel/Core/FileSystem.cpp"
"src/Hazel/Core/FileSystem.h"
"src/Hazel/Core/Input.h"
"src/Hazel/Core/KeyCodes.h"
"src/Hazel/Core/Layer.cpp"
"src/Hazel/Core/Layer.h"
"src/Hazel/Core/LayerStack.cpp"
"src/Hazel/Core/LayerStack.h"
"src/Hazel/Core/Log.cpp"
"src/Hazel/Core/Log.h"
"src/Hazel/Core/MouseCodes.h"
"src/Hazel/Core/PlatformDetection.h"
"src/Hazel/Core/Timer.h"
"src/Hazel/Core/Timestep.h"
"src/Hazel/Core/UUID.cpp"
"src/Hazel/Core/UUID.h"
"src/Hazel/Core/Window.cpp"
"src/Hazel/Core/Window.h"
"src/Hazel/Debug/Instrumentor.h"
"src/Hazel/Events/ApplicationEvent.h"
"src/Hazel/Events/Event.h"
"src/Hazel/Events/KeyEvent.h"
"src/Hazel/Events/MouseEvent.h"
"src/Hazel/ImGui/ImGuiLayer.cpp"
"src/Hazel/ImGui/ImGuiLayer.h"
"src/Hazel/Math/Math.cpp"
"src/Hazel/Math/Math.h"
"src/Hazel/Physics/Physics2D.h"
"src/Hazel/Project/Project.cpp"
"src/Hazel/Project/Project.h"
"src/Hazel/Project/ProjectSerializer.cpp"
"src/Hazel/Project/ProjectSerializer.h"
"src/Hazel/Renderer/Buffer.cpp"
"src/Hazel/Renderer/Buffer.h"
"src/Hazel/Renderer/Camera.h"
"src/Hazel/Renderer/EditorCamera.cpp"
"src/Hazel/Renderer/EditorCamera.h"
"src/Hazel/Renderer/Framebuffer.cpp"
"src/Hazel/Renderer/Framebuffer.h"
"src/Hazel/Renderer/GraphicsContext.cpp"
"src/Hazel/Renderer/GraphicsContext.h"
"src/Hazel/Renderer/OrthographicCamera.cpp"
"src/Hazel/Renderer/OrthographicCamera.h"
"src/Hazel/Renderer/OrthographicCameraController.cpp"
"src/Hazel/Renderer/OrthographicCameraController.h"
"src/Hazel/Renderer/RenderCommand.cpp"
"src/Hazel/Renderer/RenderCommand.h"
"src/Hazel/Renderer/Renderer.cpp"
"src/Hazel/Renderer/Renderer.h"
"src/Hazel/Renderer/Renderer2D.cpp"
"src/Hazel/Renderer/Renderer2D.h"
"src/Hazel/Renderer/RendererAPI.cpp"
"src/Hazel/Renderer/RendererAPI.h"
"src/Hazel/Renderer/Shader.cpp"
"src/Hazel/Renderer/Shader.h"
"src/Hazel/Renderer/Texture.cpp"
"src/Hazel/Renderer/Texture.h"
"src/Hazel/Renderer/UniformBuffer.cpp"
"src/Hazel/Renderer/UniformBuffer.h"
"src/Hazel/Renderer/VertexArray.cpp"
"src/Hazel/Renderer/VertexArray.h"
"src/Hazel/Scene/Components.h"
"src/Hazel/Scene/Entity.cpp"
"src/Hazel/Scene/Entity.h"
"src/Hazel/Scene/Scene.cpp"
"src/Hazel/Scene/Scene.h"
"src/Hazel/Scene/SceneCamera.cpp"
"src/Hazel/Scene/SceneCamera.h"
"src/Hazel/Scene/SceneSerializer.cpp"
"src/Hazel/Scene/SceneSerializer.h"
"src/Hazel/Scene/ScriptableEntity.h"
"src/Hazel/Scripting/ScriptEngineNULL.cpp"
#"src/Hazel/Scripting/ScriptEngine.cpp"
"src/Hazel/Scripting/ScriptEngine.h"
#"src/Hazel/Scripting/ScriptGlue.cpp"
"src/Hazel/Scripting/ScriptGlueNULL.cpp"
"src/Hazel/Scripting/ScriptGlue.h"
"src/Hazel/UI/UI.h"
"src/Hazel/Utils/PlatformUtils.h"
"src/Platform/OpenGL/OpenGLBuffer.cpp"
"src/Platform/OpenGL/OpenGLBuffer.h"
"src/Platform/OpenGL/OpenGLContext.cpp"
"src/Platform/OpenGL/OpenGLContext.h"
"src/Platform/OpenGL/OpenGLFramebuffer.cpp"
"src/Platform/OpenGL/OpenGLFramebuffer.h"
"src/Platform/OpenGL/OpenGLRendererAPI.cpp"
"src/Platform/OpenGL/OpenGLRendererAPI.h"
"src/Platform/OpenGL/OpenGLShader.cpp"
"src/Platform/OpenGL/OpenGLShader.h"
"src/Platform/OpenGL/OpenGLTexture.cpp"
"src/Platform/OpenGL/OpenGLTexture.h"
"src/Platform/OpenGL/OpenGLUniformBuffer.cpp"
"src/Platform/OpenGL/OpenGLUniformBuffer.h"
"src/Platform/OpenGL/OpenGLVertexArray.cpp"
"src/Platform/OpenGL/OpenGLVertexArray.h"
"src/Platform/Windows/WindowsInput.cpp"
"src/Platform/Windows/WindowsPlatformUtils.cpp"
"src/Platform/Windows/WindowsWindow.cpp"
"src/Platform/Windows/WindowsWindow.h"
"vendor/filewatch/FileWatch.hpp"
"vendor/Glad/include/glad/glad.h"
"vendor/Glad/include/KHR/khrplatform.h"
"vendor/Glad/src/glad.c"
)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}"
			#PREFIX "Source Files"
			FILES ${source_list})

add_library(${PROJECT_NAME} ${source_list})

find_package(glm REQUIRED)
find_package(spdlog 1.10.0 REQUIRED)
find_package(EnTT 3.10.1 REQUIRED)
find_package(yaml-cpp 0.7.0 REQUIRED)
find_package(box2d 2.4.1 REQUIRED)
#find_package(shaderc_util REQUIRED)
#find_package(glslang REQUIRED)
#find_package(spirv_cross_core REQUIRED)
#find_package(spirv_cross_glsl REQUIRED)
#find_package(spirv_cross_cpp REQUIRED)
#find_package(SPIRV-Tools REQUIRED)
#find_package(SPIRV-Tools-opt REQUIRED)
find_package(Stb REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${Stb_INCLUDE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC 
	glm::glm 
	spdlog::spdlog 
	EnTT::EnTT
	DockingRangeSelectImGUI
	yaml-cpp
	box2d::box2d
	

	#SPIRV-Tools-static
	#unofficial::shaderc::shaderc
	#unofficial::shaderc_util::shaderc_util
	#glslang::glslang
	#spirv-cross-core
	#spirv-cross-cpp
	#spirv-cross-glsl
	
	#SPIRV-Tools-opt
	
	)


target_precompile_headers(${PROJECT_NAME} PRIVATE "src/hzpch.h")

if(MSVC)
target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
endif()

target_compile_definitions(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:HZ_ENABLE_ASSERTS>
)

target_include_directories(${PROJECT_NAME} PUBLIC
	"${CMAKE_CURRENT_SOURCE_DIR}/src"
	"${CMAKE_CURRENT_SOURCE_DIR}/vendor/Glad/include"
	"${CMAKE_CURRENT_SOURCE_DIR}/vendor"
)

if(BUILD_SHARED_LIBS)
	target_compile_definitions(${PROJECT_NAME} INTERFACE "${PROJECT_NAME}_IMPORTS")
endif()

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

