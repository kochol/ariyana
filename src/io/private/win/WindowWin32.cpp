#include "WindowWin32.hpp"
#include <windowsx.h>
#include "io/Window.hpp"
#include "core/LockScope.hpp"
#include "core/containers/StaticArray.hpp"
#include "core/HandleManager.hpp"

namespace ari
{
	namespace io
	{
		// globals
		core::StaticArray<WindowWin32, MaxWindow>	g_Windows;
		static bool Initialized = false;

		WindowHandle FindHandle(HWND hwnd)
		{
			LOCKSCOPE;

			uint32_t i = 0;
			for(const auto& w: g_Windows)
			{
				if (w.hWnd == hwnd)
					return { core::HandleManager<WindowHandle>::GetHandleByIndex(i), i };
				i++;
			}

			return { };
		}

		// sapp globals
#ifndef WM_MOUSEHWHEEL
#define WM_MOUSEHWHEEL (0x020E)
#endif

#ifndef DPI_ENUMS_DECLARED
		typedef enum PROCESS_DPI_AWARENESS
		{
			PROCESS_DPI_UNAWARE = 0,
			PROCESS_SYSTEM_DPI_AWARE = 1,
			PROCESS_PER_MONITOR_DPI_AWARE = 2
		} PROCESS_DPI_AWARENESS;
		typedef enum MONITOR_DPI_TYPE {
			MDT_EFFECTIVE_DPI = 0,
			MDT_ANGULAR_DPI = 1,
			MDT_RAW_DPI = 2,
			MDT_DEFAULT = MDT_EFFECTIVE_DPI
		} MONITOR_DPI_TYPE;
#endif /*DPI_ENUMS_DECLARED*/

		static bool _sapp_win32_dpi_aware;
		static float _sapp_win32_content_scale;
		static float _sapp_win32_window_scale;
		static float _sapp_win32_mouse_scale;

		typedef BOOL(WINAPI* SETPROCESSDPIAWARE_T)(void);
		typedef HRESULT(WINAPI* SETPROCESSDPIAWARENESS_T)(PROCESS_DPI_AWARENESS);
		typedef HRESULT(WINAPI* GETDPIFORMONITOR_T)(HMONITOR, MONITOR_DPI_TYPE, UINT*, UINT*);
		static SETPROCESSDPIAWARE_T _sapp_win32_setprocessdpiaware;
		static SETPROCESSDPIAWARENESS_T _sapp_win32_setprocessdpiawareness;
		static GETDPIFORMONITOR_T _sapp_win32_getdpiformonitor;


		LRESULT CALLBACK win32_wndproc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			// FIXME: refresh rendering during resize with a WM_TIMER event
			WindowHandle handle = FindHandle(hWnd);
			if (handle.IsValid())
			{
				WindowWin32& window_win32 = g_Windows[handle.Index];
				if (!window_win32.win32_in_create_window) {
					switch (uMsg) {
					case WM_CLOSE:
						///* only give user a chance to intervene when sapp_quit() wasn't already called */
						//if (!_sapp.quit_ordered) {
						//	/* if window should be closed and event handling is enabled, give user code
						//		a change to intervene via sapp_cancel_quit()
						//	*/
						//	_sapp.quit_requested = true;
						//	_sapp_win32_app_event(SAPP_EVENTTYPE_QUIT_REQUESTED);
						//	/* if user code hasn't intervened, quit the app */
						//	if (_sapp.quit_requested) {
						//		_sapp.quit_ordered = true;
						//	}
						//}
						//if (_sapp.quit_ordered) {
							PostQuitMessage(0);
						//}
						return 0;
					case WM_SYSCOMMAND:
						switch (wParam & 0xFFF0) {
						case SC_SCREENSAVE:
						case SC_MONITORPOWER:
							if (window_win32.FullScreen) {
								/* disable screen saver and blanking in fullscreen mode */
								return 0;
							}
							break;
						case SC_KEYMENU:
							/* user trying to access menu via ALT */
							return 0;
						}
						break;
					case WM_ERASEBKGND:
						return 1;
//					case WM_SIZE:
//					{
//						const bool iconified = wParam == SIZE_MINIMIZED;
//						if (iconified != _sapp_win32_iconified) {
//							_sapp_win32_iconified = iconified;
//							if (iconified) {
//								_sapp_win32_app_event(SAPP_EVENTTYPE_ICONIFIED);
//							}
//							else {
//								_sapp_win32_app_event(SAPP_EVENTTYPE_RESTORED);
//							}
//						}
//						if (_sapp_win32_update_dimensions()) {
//#if defined(SOKOL_D3D11)
//							_sapp_d3d11_resize_default_render_target();
//#endif
//							_sapp_win32_app_event(SAPP_EVENTTYPE_RESIZED);
//						}
//					}
//					break;
//					case WM_SETCURSOR:
//						if (_sapp.desc.user_cursor) {
//							if (LOWORD(lParam) == HTCLIENT) {
//								_sapp_win32_app_event(SAPP_EVENTTYPE_UPDATE_CURSOR);
//								return 1;
//							}
//						}
//						break;
//					case WM_LBUTTONDOWN:
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_DOWN, SAPP_MOUSEBUTTON_LEFT);
//						break;
//					case WM_RBUTTONDOWN:
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_DOWN, SAPP_MOUSEBUTTON_RIGHT);
//						break;
//					case WM_MBUTTONDOWN:
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_DOWN, SAPP_MOUSEBUTTON_MIDDLE);
//						break;
//					case WM_LBUTTONUP:
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_UP, SAPP_MOUSEBUTTON_LEFT);
//						break;
//					case WM_RBUTTONUP:
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_UP, SAPP_MOUSEBUTTON_RIGHT);
//						break;
//					case WM_MBUTTONUP:
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_UP, SAPP_MOUSEBUTTON_MIDDLE);
//						break;
//					case WM_MOUSEMOVE:
//						_sapp.mouse_x = (float)GET_X_LPARAM(lParam) * _sapp_win32_mouse_scale;
//						_sapp.mouse_y = (float)GET_Y_LPARAM(lParam) * _sapp_win32_mouse_scale;
//						if (!_sapp.win32_mouse_tracked) {
//							_sapp.win32_mouse_tracked = true;
//							TRACKMOUSEEVENT tme;
//							memset(&tme, 0, sizeof(tme));
//							tme.cbSize = sizeof(tme);
//							tme.dwFlags = TME_LEAVE;
//							tme.hwndTrack = _sapp_win32_hwnd;
//							TrackMouseEvent(&tme);
//							_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_ENTER, SAPP_MOUSEBUTTON_INVALID);
//						}
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_MOVE, SAPP_MOUSEBUTTON_INVALID);
//						break;
//					case WM_MOUSELEAVE:
//						_sapp.win32_mouse_tracked = false;
//						_sapp_win32_mouse_event(SAPP_EVENTTYPE_MOUSE_LEAVE, SAPP_MOUSEBUTTON_INVALID);
//						break;
//					case WM_MOUSEWHEEL:
//						_sapp_win32_scroll_event(0.0f, (float)((SHORT)HIWORD(wParam)));
//						break;
//					case WM_MOUSEHWHEEL:
//						_sapp_win32_scroll_event((float)((SHORT)HIWORD(wParam)), 0.0f);
//						break;
//					case WM_CHAR:
//						_sapp_win32_char_event((uint32_t)wParam, !!(lParam & 0x40000000));
//						break;
//					case WM_KEYDOWN:
//					case WM_SYSKEYDOWN:
//						_sapp_win32_key_event(SAPP_EVENTTYPE_KEY_DOWN, (int)(HIWORD(lParam) & 0x1FF), !!(lParam & 0x40000000));
//						break;
//					case WM_KEYUP:
//					case WM_SYSKEYUP:
//						_sapp_win32_key_event(SAPP_EVENTTYPE_KEY_UP, (int)(HIWORD(lParam) & 0x1FF), false);
//						break;
					default:
						break;
					}
				}
			}
			return DefWindowProcW(hWnd, uMsg, wParam, lParam);
		}

		void win32_create_window(WindowWin32& window, const char* _title) {
			WNDCLASSW wndclassw;
			memset(&wndclassw, 0, sizeof(wndclassw));
			wndclassw.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			wndclassw.lpfnWndProc = (WNDPROC)win32_wndproc;
			wndclassw.hInstance = GetModuleHandleW(NULL);
			wndclassw.hCursor = LoadCursor(NULL, IDC_ARROW);
			wndclassw.hIcon = LoadIcon(NULL, IDI_WINLOGO);
			wndclassw.lpszClassName = L"Ariyana";
			RegisterClassW(&wndclassw);

			DWORD win_style;
			const DWORD win_ex_style = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
			RECT rect = { 0, 0, 0, 0 };
			if (window.FullScreen) {
				win_style = WS_POPUP | WS_SYSMENU | WS_VISIBLE;
				rect.right = GetSystemMetrics(SM_CXSCREEN);
				rect.bottom = GetSystemMetrics(SM_CYSCREEN);
			}
			else {
				win_style = WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SIZEBOX;
				rect.right = (int)((float)window.Width *_sapp_win32_window_scale);
				rect.bottom = (int)((float)window.Height *_sapp_win32_window_scale);
			}
			AdjustWindowRectEx(&rect, win_style, FALSE, win_ex_style);
			const int win_width = rect.right - rect.left;
			const int win_height = rect.bottom - rect.top;
			window.win32_in_create_window = true;
			window.hWnd = CreateWindowEx(
				win_ex_style,               /* dwExStyle */
				"Ariyana",                /* lpClassName */
				_title,    /* lpWindowName */
				win_style,                  /* dwStyle */
				CW_USEDEFAULT,              /* X */
				CW_USEDEFAULT,              /* Y */
				win_width,                  /* nWidth */
				win_height,                 /* nHeight */
				NULL,                       /* hWndParent */
				NULL,                       /* hMenu */
				GetModuleHandle(NULL),      /* hInstance */
				NULL);                      /* lParam */
			ShowWindow(window.hWnd, SW_SHOW);
			window.win32_in_create_window = false;
			//_sapp_win32_dc = GetDC(_sapp_win32_hwnd);
			//a_assert(_sapp_win32_dc);
			//_sapp_win32_update_dimensions();
		}

		void win32_destroy_window(void) {
			//DestroyWindow(_sapp_win32_hwnd); _sapp_win32_hwnd = 0;
			//UnregisterClassW(L"SOKOLAPP", GetModuleHandleW(NULL));
		}

		void _sapp_win32_init_dpi(WindowWin32& window) {
			a_assert(0 == _sapp_win32_setprocessdpiaware);
			a_assert(0 == _sapp_win32_setprocessdpiawareness);
			a_assert(0 == _sapp_win32_getdpiformonitor);
			HINSTANCE user32 = LoadLibraryA("user32.dll");
			if (user32) {
				_sapp_win32_setprocessdpiaware = (SETPROCESSDPIAWARE_T)GetProcAddress(user32, "SetProcessDPIAware");
			}
			HINSTANCE shcore = LoadLibraryA("shcore.dll");
			if (shcore) {
				_sapp_win32_setprocessdpiawareness = (SETPROCESSDPIAWARENESS_T)GetProcAddress(shcore, "SetProcessDpiAwareness");
				_sapp_win32_getdpiformonitor = (GETDPIFORMONITOR_T)GetProcAddress(shcore, "GetDpiForMonitor");
			}
			if (_sapp_win32_setprocessdpiawareness) {
				/* if the app didn't request HighDPI rendering, let Windows do the upscaling */
				PROCESS_DPI_AWARENESS process_dpi_awareness = PROCESS_SYSTEM_DPI_AWARE;
				_sapp_win32_dpi_aware = true;
				if (!window.HighDpi) {
					process_dpi_awareness = PROCESS_DPI_UNAWARE;
					_sapp_win32_dpi_aware = false;
				}
				_sapp_win32_setprocessdpiawareness(process_dpi_awareness);
			}
			else if (_sapp_win32_setprocessdpiaware) {
				_sapp_win32_setprocessdpiaware();
				_sapp_win32_dpi_aware = true;
			}
			/* get dpi scale factor for main monitor */
			if (_sapp_win32_getdpiformonitor && _sapp_win32_dpi_aware) {
				POINT pt = { 1, 1 };
				HMONITOR hm = MonitorFromPoint(pt, MONITOR_DEFAULTTONEAREST);
				UINT dpix, dpiy;
				HRESULT hr = _sapp_win32_getdpiformonitor(hm, MDT_EFFECTIVE_DPI, &dpix, &dpiy);
				sx_unused(hr);
				a_assert(SUCCEEDED(hr));
				/* clamp window scale to an integer factor */
				_sapp_win32_window_scale = (float)dpix / 96.0f;
			}
			else {
				_sapp_win32_window_scale = 1.0f;
			}
			if (window.HighDpi) {
				_sapp_win32_content_scale = _sapp_win32_window_scale;
				_sapp_win32_mouse_scale = 1.0f;
			}
			else {
				_sapp_win32_content_scale = 1.0f;
				_sapp_win32_mouse_scale = 1.0f / _sapp_win32_window_scale;
			}
			window.DpiScale = _sapp_win32_content_scale;
			if (user32) {
				FreeLibrary(user32);
			}
			if (shcore) {
				FreeLibrary(shcore);
			}
		}

		WindowHandle CreateAriWindow(int _width, int _height, const char* _title)
		{
			uint32_t index;
			const uint32_t handle = core::HandleManager<WindowHandle>::GetNewHandle(index);
			a_assert(index < MaxWindow);
			WindowWin32 window;
			window.Width = _width;
			window.Height = _height;

			if (!Initialized)
			{
				Initialized = true;
				_sapp_win32_init_dpi(window);
			}
			win32_create_window(window, _title);
			g_Windows[index] = window;
			return { handle, index };
		}

		bool Run()
		{
			MSG msg;
			while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (WM_QUIT == msg.message)
					return false;
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return true;
		}

	} // io

} // ari