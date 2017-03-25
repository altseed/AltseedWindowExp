#pragma once

#include "ap.Base.h"

namespace ap
{

class Window
{
public:
	Window() {}
	virtual ~Window() {}

	virtual ErrorCode Initialize(int32_t windowSizeX, int32_t windowSizeY) = 0;

	virtual bool DoEvent() = 0;

	static Window* Create();
};

}