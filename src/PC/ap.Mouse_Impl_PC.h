#pragma once

#include "../ap.Mouse.h"

namespace ap
{

	class Mouse_Impl_PC
		: public Mouse
	{
	private:
		static const int KEY_NUM = 8;

		Window*		window = nullptr;

		std::array<bool, KEY_NUM>	currentHit;
		std::array<bool, KEY_NUM>	preHit;

		float	posX = 0;
		float	posY = 0;
		float	wheel = 0;

	public:
		Mouse_Impl_PC(Window* window);
		virtual ~Mouse_Impl_PC();

		void RefreshInputState();

		void GetPosition(float& x, float& y) const override;

		void SetPosition(float x, float y) override;

		float GetWheel() const override;

		InputState GetMouseButtonState(MouseButtons button) const override;
	};

}