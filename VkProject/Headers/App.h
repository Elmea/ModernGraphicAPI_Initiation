#pragma once

#include "Window.h"

namespace Core
{
	class App
	{
	public:
		void Awake(WindowInfo windowInfo);
		void Start();
		void Update();
		void Destroy();

	private:
		Window* m_window;
	};
}