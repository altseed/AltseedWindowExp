#include "ap.Mouse_Impl_PC.h"

#include "ap.Window_Impl_PC.h"

namespace ap
{

Mouse_Impl_PC::Mouse_Impl_PC(Window* window)
	: window(window)
{

}

Mouse_Impl_PC::~Mouse_Impl_PC()
{

}

void Mouse_Impl_PC::RefreshInputState()
{
	auto w = (Window_Impl_PC*)window;

	preHit = currentHit;

	for (int32_t i = 0; i < KEY_NUM; i++)
	{
		currentHit[i] = (bool)glfwGetMouseButton(w->GetNativeWindow(), i);
	}
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