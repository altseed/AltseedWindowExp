#include "ap.Mouse_Impl_PC.h"

#include "ap.Window_Impl_PC.h"

namespace ap
{

static double wheelTemp = 0;
static bool wheelCalled = false;
static void GetWheelInternal(GLFWwindow* wHandle, double x, double y)
{
	wheelTemp = y;
	wheelCalled = true;
}

Mouse_Impl_PC::Mouse_Impl_PC(Window* window)
	: window(window)
{
	auto w = (Window_Impl_PC*)window;
	glfwSetScrollCallback(w->GetNativeWindow(), GetWheelInternal);
}

Mouse_Impl_PC::~Mouse_Impl_PC()
{

}

void Mouse_Impl_PC::RefreshInputState()
{
	auto w = (Window_Impl_PC*)window;

	double mx, my;
	glfwGetCursorPos(w->GetNativeWindow(), &mx, &my);
	posX = mx;
	posY = my;

	wheel = (wheelCalled) ? wheelTemp : 0;
	wheelCalled = false;

	preHit = currentHit;

	for (int32_t i = 0; i < KEY_NUM; i++)
	{
		currentHit[i] = (bool)glfwGetMouseButton(w->GetNativeWindow(), i);
	}
}

void Mouse_Impl_PC::GetPosition(float& x, float& y) const
{
	x = posX;
	y = posY;
}

void Mouse_Impl_PC::SetPosition(float x, float y)
{
	auto w = (Window_Impl_PC*)window;

	posX = x;
	posY = y;

	glfwSetCursorPos(w->GetNativeWindow(), x, y);
}

float Mouse_Impl_PC::GetWheel() const
{
	return wheel;
}


InputState Mouse_Impl_PC::GetMouseButtonState(MouseButtons button) const
{
	int index = (int32_t)button;
	if (currentHit[index] && preHit[index]) return InputState::Hold;
	else if (!currentHit[index] && preHit[index]) return InputState::Release;
	else if (currentHit[index] && !preHit[index]) return InputState::Push;
	else return InputState::Free;
}

}