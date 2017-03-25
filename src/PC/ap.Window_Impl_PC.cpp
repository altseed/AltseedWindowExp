#include "ap.Window_Impl_PC.h"

namespace ap
{
	void Window_Impl_PC::Terminate()
	{
		if (window != nullptr)
		{
			glfwDestroyWindow(window);
			window = nullptr;
			glfwTerminate();
		}
	}

	Window_Impl_PC::Window_Impl_PC()
	{

	}
	
	Window_Impl_PC::~Window_Impl_PC()
	{
		Terminate();
	}

	ErrorCode Window_Impl_PC::Initialize(const WindowInitializationParameter& parameter)
	{
		if (!glfwInit())
		{
			return ErrorCode::FailedInitializeWindowSystem;
		}

#ifdef __APPLE__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif

		GLFWmonitor* monitor = nullptr;
		if (parameter.IsFullscreenMode)
		{
			monitor = glfwGetPrimaryMonitor();
		}

		window = glfwCreateWindow(parameter.WindowWidth, parameter.WindowHeight, "", monitor, nullptr);
		if (window == nullptr)
		{
			glfwTerminate();
			return ErrorCode::FailedCreateWindow;
		}

		glfwSwapInterval(1);

		return ErrorCode::OK;
	}

	bool Window_Impl_PC::DoEvent()
	{
		if (window = nullptr) return false;

		if (glfwWindowShouldClose(window))
		{
			Terminate();
			return false;
		}

		glfwPollEvents();
		return true;
	}

	void Window_Impl_PC::Close()
	{
		glfwSetWindowShouldClose(window, 1);
	}
}