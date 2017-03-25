#pragma once

#include "ap.Base.h"

namespace ap
{

class Window
{
public:
	Window() {}
	virtual ~Window() {}

	virtual ErrorCode Initialize() = 0;

	static Window* Create();
};

}