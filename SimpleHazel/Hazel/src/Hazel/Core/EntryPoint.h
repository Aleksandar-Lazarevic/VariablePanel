#pragma once
#include "Hazel/Core/Base.h"
#include "Hazel/Core/Application.h"

#ifdef HZ_PLATFORM_WINDOWS

extern Hazel::Application* Hazel::CreateApplication(ApplicationCommandLineArgs args);

static auto enable_debug_memory_checker() -> void
{
#if defined(_DEBUG) && defined(WIN32)
	int flag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG); // Get current flag
	flag |= _CRTDBG_LEAK_CHECK_DF | _CRTDBG_ALLOC_MEM_DF; // Turn on leak-checking bit
	// flag |= _CRTDBG_CHECK_ALWAYS_DF;
	_CrtSetDbgFlag(flag); // Set flag to the new value
	//_crtBreakAlloc = 9496; // add to debug memory leaks
#endif
}

int main(int argc, char** argv)
{
	Hazel::Log::Init();

	HZ_PROFILE_BEGIN_SESSION("Startup", "HazelProfile-Startup.json");
	auto app = Hazel::CreateApplication({ argc, argv });
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Runtime", "HazelProfile-Runtime.json");
	app->Run();
	HZ_PROFILE_END_SESSION();

	HZ_PROFILE_BEGIN_SESSION("Shutdown", "HazelProfile-Shutdown.json");
	delete app;
	HZ_PROFILE_END_SESSION();
}

#endif
