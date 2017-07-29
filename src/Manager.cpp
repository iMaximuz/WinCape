#include <Manager.hpp>
#include <WinCape.hpp>
#include <tchar.h>
#include <map>
using namespace std;
namespace WinCape
{
	//-------------------------------------------------------------------------
	//Implementation
	//-------------------------------------------------------------------------
	class ManagerImpl
	{
		using EventKey = pair<Handle, WindowMessage>;
		//Wrap in a class?
		using EventMap = map<EventKey, EventCallback>;
		EventMap eventMap;
		//Wrap in font class?
		FontHandle applicationFont = 0;
	public:
		using EventKey = pair<Handle, WindowMessage>;
		//Wrap in a class?
		using EventMap = map<EventKey, EventCallback>;
		static ManagerImpl& Instance()
		{
			static ManagerImpl instance;
			return instance;
		}
		int startListening()
		{
			MSG msg;
			while (GetMessage(&msg, NULL, 0, 0))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return static_cast<int>(msg.wParam);
		}
		void listenEvent(const Handle& handle, const WindowMessage& message, const EventCallback& callback)
		{
			eventMap[EventKey{ handle, message }] = callback;
		}
		static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
		{
			//TODO: abstract message processing
			switch (message)
			{
			case WindowMessages::Destroy:
				DeleteObject(ManagerImpl::Instance().applicationFont);
				PostQuitMessage(0);
				break;
			case WindowMessages::Command:
			{
				Handle handle = (Handle)lParam;
				WindowMessage controlMessage = HIWORD(wParam);
				auto key = ManagerImpl::Instance().eventMap.find(EventKey{ handle, controlMessage });
				if (key != ManagerImpl::Instance().eventMap.end())
				{
					key->second(Event{ handle, wParam, lParam });
				}
				break;
			}
			default:
				return DefWindowProc(hWnd, message, wParam, lParam);
				break;
			}
			return 0;
		}
		void registerClass()
		{
			//auto wideWindowName = poolPtr(Utility::toWchar_t(Defaults::WindowName));
			//TODO wrap IDI macros in default header...
			WNDCLASSEX windowClass = {};
			windowClass.cbSize = sizeof(WNDCLASSEX);
			windowClass.style = Defaults::DefClassStyle;
			windowClass.lpfnWndProc = WndProc;
			windowClass.hInstance = Application::Instance();
			windowClass.hIcon = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
			windowClass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
			windowClass.lpszClassName = Defaults::WindowName;
			windowClass.hIconSm = LoadIcon(windowClass.hInstance, MAKEINTRESOURCE(IDI_APPLICATION));
			RegisterClassEx(&windowClass);
			//return windowClass;
		}
		Handle createHandle(const wchar_t* className, const wchar_t* text, const WindowStyle& style, const Rect& rect, const Handle& parent)
		{
			return CreateWindow(
				className,
				text,
				style,
				rect.position.x, rect.position.y,
				rect.size.x, rect.size.y,
				parent,
				NULL,
				Application::Instance(),
				NULL
			);
		}
		//Use font wrapper class...
		void defaultFont(const wchar_t* fontName)
		{
			//http://www.cplusplus.com/forum/windows/109795/
			DeleteObject(applicationFont);
			LOGFONT logFont = {};
			logFont.lfHeight = 16;
			_tcscpy(logFont.lfFaceName, fontName);
			applicationFont = CreateFontIndirect(&logFont);
		}
		FontHandle defaultFont()
		{
			return applicationFont;
		}
	};
	//-------------------------------------------------------------------------
	//Forwarding
	//-------------------------------------------------------------------------
	Manager::Manager() {}
	Manager& Manager::Instance()
	{
		static Manager instance;
		return instance;
	}
	int Manager::startListening()
	{
		return ManagerImpl::Instance().startListening();
	}
	void Manager::listenEvent(const Handle& handle, const WindowMessage& message, const EventCallback& callback)
	{
		ManagerImpl::Instance().listenEvent(handle, message, callback);
	}
	void Manager::registerClass()
	{
		ManagerImpl::Instance().registerClass();
	}
	Handle Manager::createHandle(const wchar_t* className, const wchar_t* text, const WindowStyle& style, const Rect& rect, const Handle& parent)
	{
		return ManagerImpl::Instance().createHandle(className, text, style, rect, parent);
	}
	void Manager::defaultFont(const wchar_t* fontName)
	{
		ManagerImpl::Instance().defaultFont(fontName);
	}
	FontHandle Manager::defaultFont()
	{
		return ManagerImpl::Instance().defaultFont();
	}
}