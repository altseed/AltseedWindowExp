#pragma once

#include "ap.Base.h"

namespace ap
{

class Window
{
public:
	Window() {}
	virtual ~Window() {}

	virtual ErrorCode Initialize(const WindowInitializationParameter& parameter) = 0;

	virtual bool DoEvent() = 0;

	virtual void Close() {}

	virtual void* GetHandle() const { return nullptr; }

	/**
		@brief	Show back buffer.
		@note
		This function is for OpenGL
	*/
	virtual void Present() {}

	/**
	@brief	Make context current.
	@note
	This function is for OpenGL
	*/
	virtual void MakeContextCurrent() {}

	/**
	@brief	Make context none.
	@note
	This function is for OpenGL
	*/
	virtual void MakeContextNone() {}

	static Window* Create();
};

}