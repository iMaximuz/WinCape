#ifndef DEFAULTS_HPP
#define DEFAULTS_HPP
#include "defines.hpp"
namespace WinCape
{
	struct Defaults
	{
		static constexpr const char* WindowName = "WinCape Window";
		static constexpr Rect WindowRect = Rect{ 100, 100, 800, 600 };
		static constexpr WindowStyle DefWindowStyle = WindowStyles::OverlappedWindow;
		static constexpr ClassStyle DefClassStyle  = ClassStyles::HorizontalRedraw | ClassStyles::VerticalRedraw;
		static constexpr ShowCommand DefShowCommand = ShowCommands::ShowDefault;
	};
}
#endif // !DEFAULTS_HPP