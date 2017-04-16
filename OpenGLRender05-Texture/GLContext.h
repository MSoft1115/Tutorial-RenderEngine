#pragma once

#include "XCore.h"
#include "GLUtil.h"

namespace X {

	class GLContext
	{
	public:
		GLContext(HWND hWnd, int w, int h);
		~GLContext();

		void
			Resize(int w, int h);
		void
			Present();

	protected:
		HWND m_hwnd;
		HDC m_hdc;
		HGLRC m_glrc;
	};

}