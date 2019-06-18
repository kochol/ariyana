#include "io/Window.hpp"
#include "core/StaticArray.hpp"

namespace ari
{
	namespace io
	{
		extern core::StaticArray<Window, MaxWindow>	g_Windows;

		core::Ptr<Window> CreateWindow(int _width, int _height, const char* _title)
		{
			return { nullptr };
		}

	} // io

} // ari