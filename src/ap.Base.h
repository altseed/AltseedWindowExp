
#pragma once

#include <stdio.h>
#include <stdint.h>
#include <array>

#ifdef __ANDROID__
#include <jni.h>
#include <errno.h>

#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

#include <android/log.h>

#else

// GLFW
#if _WIN32
#define GLFW_EXPOSE_NATIVE_WIN32
#define GLFW_EXPOSE_NATIVE_WGL
#elif __APPLE__
#define GLFW_EXPOSE_NATIVE_COCOA
#define GLFW_EXPOSE_NATIVE_NSGL
#else
#define GLFW_EXPOSE_NATIVE_X11
#define GLFW_EXPOSE_NATIVE_GLX
#include <X11/extensions/Xrandr.h>
#endif

#include <GLFW/glfw3.h>

#if _WIN32
#include <GLFW/glfw3native.h>
#endif

#endif

namespace ap
{
	enum class ErrorCode
	{
		OK,
		FailedInitializeWindowSystem,
		FailedCreateWindow,
		Unknown,
	};

	struct WindowInitializationParameter
	{
		int32_t	WindowWidth = 0;
		int32_t WindowHeight = 0;
		std::array<void*, 3> UserData;
	};
}