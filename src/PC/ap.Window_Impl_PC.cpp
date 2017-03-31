#include "ap.Window_Impl_PC.h"

#include <codecvt>

namespace ap
{
	// http://hasenpfote36.blogspot.jp/2016/09/stdcodecvt.html
	static constexpr std::codecvt_mode mode = std::codecvt_mode::little_endian;

	static std::string utf16_to_utf8(const std::u16string& s)
	{
#if defined(_MSC_VER) && (_MSC_VER <= 1900)
		std::wstring_convert<std::codecvt_utf8_utf16<std::uint16_t, 0x10ffff, mode>, std::uint16_t> conv;
		auto p = reinterpret_cast<const std::uint16_t*>(s.c_str());
		return conv.to_bytes(p, p + s.length());
#else
		std::wstring_convert<std::codecvt_utf8_utf16<char16_t, 0x10ffff, mode>, char16_t> conv;
		return conv.to_bytes(s);
#endif
	}

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

		if (parameter.GraphicsDevice == GraphicsDeviceType::OpenGL)
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
		}
		else
		{
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

		if (parameter.ColorSpace == ColorSpaceType::LinearSpace)
		{
			glfwWindowHint(GLFW_SRGB_CAPABLE, GL_TRUE);
		}

		glfwWindowHint(GLFW_VISIBLE, GL_FALSE);

		GLFWmonitor* monitor = nullptr;
		if (parameter.IsFullscreenMode)
		{
			monitor = glfwGetPrimaryMonitor();
		}

		window = glfwCreateWindow(parameter.WindowWidth, parameter.WindowHeight, utf16_to_utf8(parameter.Title).c_str(), monitor, nullptr);
		if (window == nullptr)
		{
			glfwTerminate();
			return ErrorCode::FailedCreateWindow;
		}

		glfwSwapInterval(1);

		// Centering
		if (!parameter.IsFullscreenMode && parameter.WindowPosition == WindowPositionType::Centering)
		{
			auto monitor = glfwGetPrimaryMonitor();
			
			if (monitor != nullptr)
			{
				auto videomode = glfwGetVideoMode(monitor);

				if (videomode != nullptr)
				{
					int32_t x = 0;
					int32_t y = 0;

					glfwGetMonitorPos(monitor, &x, &y);

					auto offset_x = (videomode->width - parameter.WindowWidth) / 2 + x;
					auto offset_y = (videomode->height - parameter.WindowHeight) / 2 + y;

					glfwSetWindowPos(window, offset_x, offset_y);
				}
			}
		}

		glfwShowWindow(window);

		return ErrorCode::OK;
	}

	bool Window_Impl_PC::DoEvent()
	{
		if (window == nullptr) return false;

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

	void* Window_Impl_PC::GetHandle() const
	{
		return glfwGetWin32Window(window);
	}
}