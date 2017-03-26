
#include <stdio.h>

#include <ap.Window.h>
#include <ap.Joystick.h>

#ifdef _WIN64

#ifdef _DEBUG
#pragma comment(lib,"x64/Debug/glfw3.lib")
#else
#pragma comment(lib,"x64/Release/glfw3.lib")
#endif

#else

#ifdef _DEBUG
#pragma comment(lib,"x86/Debug/glfw3.lib")
#else
#pragma comment(lib,"x86/Release/glfw3.lib")
#endif

#endif

#pragma comment(lib,"opengl32.lib")

int main()
{
	auto window = ap::Window::Create();
	
	ap::WindowInitializationParameter param;
	param.WindowWidth = 640;
	param.WindowHeight = 480;

	window->Initialize(param);

	auto joystick = ap::Joystick::Create(window);

	while (window->DoEvent())
	{
		joystick->RefreshInputState();
	}

	delete joystick;
	delete window;

	return 0;
}