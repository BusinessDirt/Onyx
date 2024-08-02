#pragma once

#include "Onyx/Core/Base.h"

extern Onyx::Application* Onyx::CreateApplication(Onyx::ApplicationCommandLineArgs args);

int Entrypoint(int argc, char** argv)
{
	Onyx::Log::Init();

	Onyx::Application* app = Onyx::CreateApplication({ argc, argv });
	app->Run();
	delete app;

	return 0;
}

#ifdef ONYX_PLATFORM_WINDOWS

#ifdef ONYX_DIST

#include <Windows.h>

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow)
{
	return Entrypoint(__argc, __argv);
}

#else

int main(int argc, char** argv) 
{
	return Entrypoint(argc, argv);
}

#endif

#else
#error OS not supported
#endif