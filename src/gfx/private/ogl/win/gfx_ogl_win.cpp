#include "gfx/gfx.hpp"
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include "io/Window.hpp"
#include "../gfx_ogl.hpp"
#include "core/containers/StaticArray.hpp"

#define WGL_NUMBER_PIXEL_FORMATS_ARB 0x2000
#define WGL_SUPPORT_OPENGL_ARB 0x2010
#define WGL_DRAW_TO_WINDOW_ARB 0x2001
#define WGL_PIXEL_TYPE_ARB 0x2013
#define WGL_TYPE_RGBA_ARB 0x202b
#define WGL_ACCELERATION_ARB 0x2003
#define WGL_NO_ACCELERATION_ARB 0x2025
#define WGL_RED_BITS_ARB 0x2015
#define WGL_RED_SHIFT_ARB 0x2016
#define WGL_GREEN_BITS_ARB 0x2017
#define WGL_GREEN_SHIFT_ARB 0x2018
#define WGL_BLUE_BITS_ARB 0x2019
#define WGL_BLUE_SHIFT_ARB 0x201a
#define WGL_ALPHA_BITS_ARB 0x201b
#define WGL_ALPHA_SHIFT_ARB 0x201c
#define WGL_ACCUM_BITS_ARB 0x201d
#define WGL_ACCUM_RED_BITS_ARB 0x201e
#define WGL_ACCUM_GREEN_BITS_ARB 0x201f
#define WGL_ACCUM_BLUE_BITS_ARB 0x2020
#define WGL_ACCUM_ALPHA_BITS_ARB 0x2021
#define WGL_DEPTH_BITS_ARB 0x2022
#define WGL_STENCIL_BITS_ARB 0x2023
#define WGL_AUX_BUFFERS_ARB 0x2024
#define WGL_STEREO_ARB 0x2012
#define WGL_DOUBLE_BUFFER_ARB 0x2011
#define WGL_SAMPLES_ARB 0x2042
#define WGL_FRAMEBUFFER_SRGB_CAPABLE_ARB 0x20a9
#define WGL_CONTEXT_DEBUG_BIT_ARB 0x00000001
#define WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB 0x00000002
#define WGL_CONTEXT_PROFILE_MASK_ARB 0x9126
#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB 0x00000001
#define WGL_CONTEXT_COMPATIBILITY_PROFILE_BIT_ARB 0x00000002
#define WGL_CONTEXT_MAJOR_VERSION_ARB 0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB 0x2092
#define WGL_CONTEXT_FLAGS_ARB 0x2094
#define WGL_CONTEXT_ROBUST_ACCESS_BIT_ARB 0x00000004
#define WGL_LOSE_CONTEXT_ON_RESET_ARB 0x8252
#define WGL_CONTEXT_RESET_NOTIFICATION_STRATEGY_ARB 0x8256
#define WGL_NO_RESET_NOTIFICATION_ARB 0x8261
#define WGL_CONTEXT_RELEASE_BEHAVIOR_ARB 0x2097
#define WGL_CONTEXT_RELEASE_BEHAVIOR_NONE_ARB 0
#define WGL_CONTEXT_RELEASE_BEHAVIOR_FLUSH_ARB 0x2098
#define WGL_COLORSPACE_EXT 0x309d
#define WGL_COLORSPACE_SRGB_EXT 0x3089
#define ERROR_INVALID_VERSION_ARB 0x2095
#define ERROR_INVALID_PROFILE_ARB 0x2096
#define ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB 0x2054
typedef BOOL(WINAPI * PFNWGLSWAPINTERVALEXTPROC)(int);
typedef BOOL(WINAPI* PFNWGLGETPIXELFORMATATTRIBIVARBPROC)(HDC, int, int, UINT, const int*, int*);
typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGEXTPROC)(void);
typedef const char* (WINAPI* PFNWGLGETEXTENSIONSSTRINGARBPROC)(HDC);
typedef HGLRC(WINAPI* PFNWGLCREATECONTEXTATTRIBSARBPROC)(HDC, HGLRC, const int*);
typedef HGLRC(WINAPI* PFN_wglCreateContext)(HDC);
typedef BOOL(WINAPI* PFN_wglDeleteContext)(HGLRC);
typedef PROC(WINAPI* PFN_wglGetProcAddress)(LPCSTR);
typedef HDC(WINAPI* PFN_wglGetCurrentDC)(void);
typedef BOOL(WINAPI* PFN_wglMakeCurrent)(HDC, HGLRC);
static HINSTANCE _sapp_opengl32;
static PFN_wglCreateContext _sapp_wglCreateContext;
static PFN_wglDeleteContext _sapp_wglDeleteContext;
static PFN_wglGetProcAddress _sapp_wglGetProcAddress;
static PFN_wglGetCurrentDC _sapp_wglGetCurrentDC;
static PFN_wglMakeCurrent _sapp_wglMakeCurrent;
static PFNWGLSWAPINTERVALEXTPROC _sapp_SwapIntervalEXT;
static PFNWGLGETPIXELFORMATATTRIBIVARBPROC _sapp_GetPixelFormatAttribivARB;
static PFNWGLGETEXTENSIONSSTRINGEXTPROC _sapp_GetExtensionsStringEXT;
static PFNWGLGETEXTENSIONSSTRINGARBPROC _sapp_GetExtensionsStringARB;
static PFNWGLCREATECONTEXTATTRIBSARBPROC _sapp_CreateContextAttribsARB;
static bool _sapp_ext_swap_control;
static bool _sapp_arb_multisample;
static bool _sapp_arb_pixel_format;
static bool _sapp_arb_create_context;
static bool _sapp_arb_create_context_profile;
static HWND _sapp_win32_msg_hwnd;
static HDC _sapp_win32_msg_dc;

ari::core::StaticArray<HGLRC, ari::io::MaxWindow>	g_GlContexts;

void _sapp_fail(const char* msg) {
	/*if (_sapp.desc.fail_cb) {
		_sapp.desc.fail_cb(msg);
	}
	else if (_sapp.desc.fail_userdata_cb) {
		_sapp.desc.fail_userdata_cb(msg, _sapp.desc.user_data);
	}
	else {
		SOKOL_LOG(msg);
	}*/
	abort();
}


void _sapp_wgl_init(void) {
	_sapp_opengl32 = LoadLibraryA("opengl32.dll");
	if (!_sapp_opengl32) {
		_sapp_fail("Failed to load opengl32.dll\n");
	}
	a_assert(_sapp_opengl32);
	_sapp_wglCreateContext = (PFN_wglCreateContext)GetProcAddress(_sapp_opengl32, "wglCreateContext");
	a_assert(_sapp_wglCreateContext);
	_sapp_wglDeleteContext = (PFN_wglDeleteContext)GetProcAddress(_sapp_opengl32, "wglDeleteContext");
	a_assert(_sapp_wglDeleteContext);
	_sapp_wglGetProcAddress = (PFN_wglGetProcAddress)GetProcAddress(_sapp_opengl32, "wglGetProcAddress");
	a_assert(_sapp_wglGetProcAddress);
	_sapp_wglGetCurrentDC = (PFN_wglGetCurrentDC)GetProcAddress(_sapp_opengl32, "wglGetCurrentDC");
	a_assert(_sapp_wglGetCurrentDC);
	_sapp_wglMakeCurrent = (PFN_wglMakeCurrent)GetProcAddress(_sapp_opengl32, "wglMakeCurrent");
	a_assert(_sapp_wglMakeCurrent);

	_sapp_win32_msg_hwnd = CreateWindowExW(WS_EX_OVERLAPPEDWINDOW,
		L"Ariyana",
		L"ariyana-app message window",
		WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
		0, 0, 1, 1,
		NULL, NULL,
		GetModuleHandleW(NULL),
		NULL);
	if (!_sapp_win32_msg_hwnd) {
		_sapp_fail("Win32: failed to create helper window!\n");
	}
	ShowWindow(_sapp_win32_msg_hwnd, SW_HIDE);
	MSG msg;
	while (PeekMessageW(&msg, _sapp_win32_msg_hwnd, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessageW(&msg);
	}
	_sapp_win32_msg_dc = GetDC(_sapp_win32_msg_hwnd);
	if (!_sapp_win32_msg_dc) {
		_sapp_fail("Win32: failed to obtain helper window DC!\n");
	}
}

void _sapp_wgl_shutdown(void) {
	a_assert(_sapp_opengl32 && _sapp_win32_msg_hwnd);
	DestroyWindow(_sapp_win32_msg_hwnd); _sapp_win32_msg_hwnd = 0;
	FreeLibrary(_sapp_opengl32); _sapp_opengl32 = 0;
}

bool _sapp_wgl_has_ext(const char* ext, const char* extensions) {
	a_assert(ext && extensions);
	const char* start = extensions;
	while (true) {
		const char* where = strstr(start, ext);
		if (!where) {
			return false;
		}
		const char* terminator = where + strlen(ext);
		if ((where == start) || (*(where - 1) == ' ')) {
			if (*terminator == ' ' || *terminator == '\0') {
				break;
			}
		}
		start = terminator;
	}
	return true;
}

bool _sapp_wgl_ext_supported(const char* ext) {
	a_assert(ext);
	if (_sapp_GetExtensionsStringEXT) {
		const char* extensions = _sapp_GetExtensionsStringEXT();
		if (extensions) {
			if (_sapp_wgl_has_ext(ext, extensions)) {
				return true;
			}
		}
	}
	if (_sapp_GetExtensionsStringARB) {
		const char* extensions = _sapp_GetExtensionsStringARB(_sapp_wglGetCurrentDC());
		if (extensions) {
			if (_sapp_wgl_has_ext(ext, extensions)) {
				return true;
			}
		}
	}
	return false;
}

void _sapp_wgl_load_extensions(void) {
	a_assert(_sapp_win32_msg_dc);
	PIXELFORMATDESCRIPTOR pfd;
	memset(&pfd, 0, sizeof(pfd));
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 24;
	if (!SetPixelFormat(_sapp_win32_msg_dc, ChoosePixelFormat(_sapp_win32_msg_dc, &pfd), &pfd)) {
		_sapp_fail("WGL: failed to set pixel format for dummy context\n");
	}
	HGLRC rc = _sapp_wglCreateContext(_sapp_win32_msg_dc);
	if (!rc) {
		_sapp_fail("WGL: Failed to create dummy context\n");
	}
	if (!_sapp_wglMakeCurrent(_sapp_win32_msg_dc, rc)) {
		_sapp_fail("WGL: Failed to make context current\n");
	}
	_sapp_GetExtensionsStringEXT = (PFNWGLGETEXTENSIONSSTRINGEXTPROC)_sapp_wglGetProcAddress("wglGetExtensionsStringEXT");
	_sapp_GetExtensionsStringARB = (PFNWGLGETEXTENSIONSSTRINGARBPROC)_sapp_wglGetProcAddress("wglGetExtensionsStringARB");
	_sapp_CreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)_sapp_wglGetProcAddress("wglCreateContextAttribsARB");
	_sapp_SwapIntervalEXT = (PFNWGLSWAPINTERVALEXTPROC)_sapp_wglGetProcAddress("wglSwapIntervalEXT");
	_sapp_GetPixelFormatAttribivARB = (PFNWGLGETPIXELFORMATATTRIBIVARBPROC)_sapp_wglGetProcAddress("wglGetPixelFormatAttribivARB");
	_sapp_arb_multisample = _sapp_wgl_ext_supported("WGL_ARB_multisample");
	_sapp_arb_create_context = _sapp_wgl_ext_supported("WGL_ARB_create_context");
	_sapp_arb_create_context_profile = _sapp_wgl_ext_supported("WGL_ARB_create_context_profile");
	_sapp_ext_swap_control = _sapp_wgl_ext_supported("WGL_EXT_swap_control");
	_sapp_arb_pixel_format = _sapp_wgl_ext_supported("WGL_ARB_pixel_format");
	_sapp_wglMakeCurrent(_sapp_win32_msg_dc, 0);
	_sapp_wglDeleteContext(rc);
}

int _sapp_wgl_attrib(int pixel_format, int attrib, HDC hdc) {
	a_assert(_sapp_arb_pixel_format);
	int value = 0;
	if (!_sapp_GetPixelFormatAttribivARB(hdc, pixel_format, 0, 1, &attrib, &value)) {
		_sapp_fail("WGL: Failed to retrieve pixel format attribute\n");
	}
	return value;
}

int _sapp_wgl_find_pixel_format(const ari::io::WindowHandle& handle, ari::gfx::gfxSetup& gfx_setup) 
{
	HDC hdc = (HDC)ari::io::GetWin32HDC(handle);
	a_assert(hdc);
	a_assert(_sapp_arb_pixel_format);
	const _sapp_gl_fbconfig* closest;

	int native_count = _sapp_wgl_attrib(1, WGL_NUMBER_PIXEL_FORMATS_ARB, hdc);
	_sapp_gl_fbconfig* usable_configs = (_sapp_gl_fbconfig*)ari::core::Memory::
		Alloc(native_count * sizeof(_sapp_gl_fbconfig));
	ari::core::Memory::Fill(usable_configs, native_count * sizeof(_sapp_gl_fbconfig), 0);
	int usable_count = 0;
	for (int i = 0; i < native_count; i++) {
		const int n = i + 1;
		_sapp_gl_fbconfig* u = usable_configs + usable_count;
		_sapp_gl_init_fbconfig(u);
		if (!_sapp_wgl_attrib(n, WGL_SUPPORT_OPENGL_ARB, hdc) || !_sapp_wgl_attrib(n, WGL_DRAW_TO_WINDOW_ARB, hdc)) 
		{
			continue;
		}
		if (_sapp_wgl_attrib(n, WGL_PIXEL_TYPE_ARB, hdc) != WGL_TYPE_RGBA_ARB) {
			continue;
		}
		if (_sapp_wgl_attrib(n, WGL_ACCELERATION_ARB, hdc) == WGL_NO_ACCELERATION_ARB) {
			continue;
		}
		u->red_bits = _sapp_wgl_attrib(n, WGL_RED_BITS_ARB, hdc);
		u->green_bits = _sapp_wgl_attrib(n, WGL_GREEN_BITS_ARB, hdc);
		u->blue_bits = _sapp_wgl_attrib(n, WGL_BLUE_BITS_ARB, hdc);
		u->alpha_bits = _sapp_wgl_attrib(n, WGL_ALPHA_BITS_ARB, hdc);
		u->depth_bits = _sapp_wgl_attrib(n, WGL_DEPTH_BITS_ARB, hdc);
		u->stencil_bits = _sapp_wgl_attrib(n, WGL_STENCIL_BITS_ARB, hdc);
		if (_sapp_wgl_attrib(n, WGL_DOUBLE_BUFFER_ARB, hdc)) {
			u->doublebuffer = true;
		}
		if (_sapp_arb_multisample) {
			u->samples = _sapp_wgl_attrib(n, WGL_SAMPLES_ARB, hdc);
		}
		u->handle = n;
		usable_count++;
	}
	a_assert(usable_count > 0);
	_sapp_gl_fbconfig desired;
	_sapp_gl_init_fbconfig(&desired);
	desired.red_bits = 8;
	desired.green_bits = 8;
	desired.blue_bits = 8;
	desired.alpha_bits = 8;
	desired.depth_bits = 24;
	desired.stencil_bits = 8;
	desired.doublebuffer = true;
	desired.samples = gfx_setup.sample_count > 1 ? gfx_setup.sample_count : 0;
	closest = _sapp_gl_choose_fbconfig(&desired, usable_configs, usable_count);
	int pixel_format = 0;
	if (closest) {
		pixel_format = (int)closest->handle;
	}
	ari::core::Memory::Free(usable_configs);
	return pixel_format;
}

void _sapp_wgl_create_context(const ari::io::WindowHandle& handle, ari::gfx::gfxSetup& gfx_setup) {
	HDC hdc = (HDC)ari::io::GetWin32HDC(handle);
	int pixel_format = _sapp_wgl_find_pixel_format(handle, gfx_setup);
	if (0 == pixel_format) {
		_sapp_fail("WGL: Didn't find matching pixel format.\n");
	}
	PIXELFORMATDESCRIPTOR pfd;
	if (!DescribePixelFormat(hdc, pixel_format, sizeof(pfd), &pfd)) {
		_sapp_fail("WGL: Failed to retrieve PFD for selected pixel format!\n");
	}
	if (!SetPixelFormat(hdc, pixel_format, &pfd)) {
		_sapp_fail("WGL: Failed to set selected pixel format!\n");
	}
	if (!_sapp_arb_create_context) {
		_sapp_fail("WGL: ARB_create_context required!\n");
	}
	if (!_sapp_arb_create_context_profile) {
		_sapp_fail("WGL: ARB_create_context_profile required!\n");
	}
	const int attrs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0, 0
	};
	g_GlContexts[handle.Index] = _sapp_CreateContextAttribsARB(hdc, 0, attrs);
	if (!g_GlContexts[handle.Index]) {
		const DWORD err = GetLastError();
		if (err == (0xc0070000 | ERROR_INVALID_VERSION_ARB)) {
			_sapp_fail("WGL: Driver does not support OpenGL version 3.3\n");
		}
		else if (err == (0xc0070000 | ERROR_INVALID_PROFILE_ARB)) {
			_sapp_fail("WGL: Driver does not support the requested OpenGL profile");
		}
		else if (err == (0xc0070000 | ERROR_INCOMPATIBLE_DEVICE_CONTEXTS_ARB)) {
			_sapp_fail("WGL: The share context is not compatible with the requested context");
		}
		else {
			_sapp_fail("WGL: Failed to create OpenGL context");
		}
	}
	_sapp_wglMakeCurrent(hdc, g_GlContexts[handle.Index]);
	if (_sapp_ext_swap_control) {
		/* FIXME: DwmIsCompositionEnabled() (see GLFW) */
		_sapp_SwapIntervalEXT(gfx_setup.swap_interval);
	}
}

void _sapp_wgl_destroy_context(const ari::io::WindowHandle& handle)
{
	// TODO: Destroy the window context
	a_assert(g_GlContexts[handle.Index]);
	_sapp_wglDeleteContext(g_GlContexts[handle.Index]);
	g_GlContexts[handle.Index] = nullptr;
}

void _sapp_wgl_swap_buffers(const ari::io::WindowHandle& handle) 
{
	a_assert(handle.IsValid());
	HDC hdc = (HDC)ari::io::GetWin32HDC(handle);
	a_assert(hdc);
	/* FIXME: DwmIsCompositionEnabled? (see GLFW) */
	SwapBuffers(hdc);
}

/* NOTE: the optional GL loader only contains the GL constants and functions required for sokol_gfx.h, if you need
more, you'll need to use you own gl header-generator/loader
*/
#if !defined(SOKOL_WIN32_NO_GL_LOADER)
#define __gl_h_ 1
#define __gl32_h_ 1
#define __gl31_h_ 1
#define __GL_H__ 1
#define __glext_h_ 1
#define __GLEXT_H_ 1
#define __gltypes_h_ 1
#define __glcorearb_h_ 1
#define __gl_glcorearb_h_ 1
#define GL_APIENTRY APIENTRY

typedef unsigned int  GLenum;
typedef unsigned int  GLuint;
typedef int  GLsizei;
typedef char  GLchar;
typedef ptrdiff_t  GLintptr;
typedef ptrdiff_t  GLsizeiptr;
typedef double  GLclampd;
typedef unsigned short  GLushort;
typedef unsigned char  GLubyte;
typedef unsigned char  GLboolean;
typedef uint64_t  GLuint64;
typedef double  GLdouble;
typedef unsigned short  GLhalf;
typedef float  GLclampf;
typedef unsigned int  GLbitfield;
typedef signed char  GLbyte;
typedef short  GLshort;
typedef void  GLvoid;
typedef int64_t  GLint64;
typedef float  GLfloat;
typedef struct __GLsync* GLsync;
typedef int  GLint;
#define GL_INT_2_10_10_10_REV 0x8D9F
#define GL_R32F 0x822E
#define GL_PROGRAM_POINT_SIZE 0x8642
#define GL_STENCIL_ATTACHMENT 0x8D20
#define GL_DEPTH_ATTACHMENT 0x8D00
#define GL_COLOR_ATTACHMENT2 0x8CE2
#define GL_COLOR_ATTACHMENT0 0x8CE0
#define GL_R16F 0x822D
#define GL_COLOR_ATTACHMENT22 0x8CF6
#define GL_DRAW_FRAMEBUFFER 0x8CA9
#define GL_FRAMEBUFFER_COMPLETE 0x8CD5
#define GL_NUM_EXTENSIONS 0x821D
#define GL_INFO_LOG_LENGTH 0x8B84
#define GL_VERTEX_SHADER 0x8B31
#define GL_INCR 0x1E02
#define GL_DYNAMIC_DRAW 0x88E8
#define GL_STATIC_DRAW 0x88E4
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Z 0x8519
#define GL_TEXTURE_CUBE_MAP 0x8513
#define GL_FUNC_SUBTRACT 0x800A
#define GL_FUNC_REVERSE_SUBTRACT 0x800B
#define GL_CONSTANT_COLOR 0x8001
#define GL_DECR_WRAP 0x8508
#define GL_R8 0x8229
#define GL_LINEAR_MIPMAP_LINEAR 0x2703
#define GL_ELEMENT_ARRAY_BUFFER 0x8893
#define GL_SHORT 0x1402
#define GL_DEPTH_TEST 0x0B71
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Y 0x8518
#define GL_LINK_STATUS 0x8B82
#define GL_TEXTURE_CUBE_MAP_POSITIVE_Y 0x8517
#define GL_SAMPLE_ALPHA_TO_COVERAGE 0x809E
#define GL_RGBA16F 0x881A
#define GL_CONSTANT_ALPHA 0x8003
#define GL_READ_FRAMEBUFFER 0x8CA8
#define GL_TEXTURE0 0x84C0
#define GL_TEXTURE_MIN_LOD 0x813A
#define GL_CLAMP_TO_EDGE 0x812F
#define GL_UNSIGNED_SHORT_5_6_5 0x8363
#define GL_TEXTURE_WRAP_R 0x8072
#define GL_UNSIGNED_SHORT_5_5_5_1 0x8034
#define GL_NEAREST_MIPMAP_NEAREST 0x2700
#define GL_UNSIGNED_SHORT_4_4_4_4 0x8033
#define GL_SRC_ALPHA_SATURATE 0x0308
#define GL_STREAM_DRAW 0x88E0
#define GL_ONE 1
#define GL_NEAREST_MIPMAP_LINEAR 0x2702
#define GL_RGB10_A2 0x8059
#define GL_RGBA8 0x8058
#define GL_COLOR_ATTACHMENT1 0x8CE1
#define GL_RGBA4 0x8056
#define GL_RGB8 0x8051
#define GL_ARRAY_BUFFER 0x8892
#define GL_STENCIL 0x1802
#define GL_TEXTURE_2D 0x0DE1
#define GL_DEPTH 0x1801
#define GL_FRONT 0x0404
#define GL_STENCIL_BUFFER_BIT 0x00000400
#define GL_REPEAT 0x2901
#define GL_RGBA 0x1908
#define GL_TEXTURE_CUBE_MAP_POSITIVE_X 0x8515
#define GL_DECR 0x1E03
#define GL_FRAGMENT_SHADER 0x8B30
#define GL_FLOAT 0x1406
#define GL_TEXTURE_MAX_LOD 0x813B
#define GL_DEPTH_COMPONENT 0x1902
#define GL_ONE_MINUS_DST_ALPHA 0x0305
#define GL_COLOR 0x1800
#define GL_TEXTURE_2D_ARRAY 0x8C1A
#define GL_TRIANGLES 0x0004
#define GL_UNSIGNED_BYTE 0x1401
#define GL_TEXTURE_MAG_FILTER 0x2800
#define GL_ONE_MINUS_CONSTANT_ALPHA 0x8004
#define GL_NONE 0
#define GL_SRC_COLOR 0x0300
#define GL_BYTE 0x1400
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_Z 0x851A
#define GL_LINE_STRIP 0x0003
#define GL_TEXTURE_3D 0x806F
#define GL_CW 0x0900
#define GL_LINEAR 0x2601
#define GL_RENDERBUFFER 0x8D41
#define GL_GEQUAL 0x0206
#define GL_COLOR_BUFFER_BIT 0x00004000
#define GL_RGBA32F 0x8814
#define GL_BLEND 0x0BE2
#define GL_ONE_MINUS_SRC_ALPHA 0x0303
#define GL_ONE_MINUS_CONSTANT_COLOR 0x8002
#define GL_TEXTURE_WRAP_T 0x2803
#define GL_TEXTURE_WRAP_S 0x2802
#define GL_TEXTURE_MIN_FILTER 0x2801
#define GL_LINEAR_MIPMAP_NEAREST 0x2701
#define GL_EXTENSIONS 0x1F03
#define GL_NO_ERROR 0
#define GL_REPLACE 0x1E01
#define GL_KEEP 0x1E00
#define GL_CCW 0x0901
#define GL_TEXTURE_CUBE_MAP_NEGATIVE_X 0x8516
#define GL_RGB 0x1907
#define GL_TRIANGLE_STRIP 0x0005
#define GL_FALSE 0
#define GL_ZERO 0
#define GL_CULL_FACE 0x0B44
#define GL_INVERT 0x150A
#define GL_UNSIGNED_INT 0x1405
#define GL_UNSIGNED_SHORT 0x1403
#define GL_NEAREST 0x2600
#define GL_SCISSOR_TEST 0x0C11
#define GL_LEQUAL 0x0203
#define GL_STENCIL_TEST 0x0B90
#define GL_DITHER 0x0BD0
#define GL_DEPTH_COMPONENT16 0x81A5
#define GL_EQUAL 0x0202
#define GL_FRAMEBUFFER 0x8D40
#define GL_RGB5 0x8050
#define GL_LINES 0x0001
#define GL_DEPTH_BUFFER_BIT 0x00000100
#define GL_SRC_ALPHA 0x0302
#define GL_INCR_WRAP 0x8507
#define GL_LESS 0x0201
#define GL_MULTISAMPLE 0x809D
#define GL_FRAMEBUFFER_BINDING 0x8CA6
#define GL_BACK 0x0405
#define GL_ALWAYS 0x0207
#define GL_FUNC_ADD 0x8006
#define GL_ONE_MINUS_DST_COLOR 0x0307
#define GL_NOTEQUAL 0x0205
#define GL_DST_COLOR 0x0306
#define GL_COMPILE_STATUS 0x8B81
#define GL_RED 0x1903
#define GL_COLOR_ATTACHMENT3 0x8CE3
#define GL_DST_ALPHA 0x0304
#define GL_RGB5_A1 0x8057
#define GL_GREATER 0x0204
#define GL_POLYGON_OFFSET_FILL 0x8037
#define GL_TRUE 1
#define GL_NEVER 0x0200
#define GL_POINTS 0x0000
#define GL_ONE_MINUS_SRC_COLOR 0x0301
#define GL_MIRRORED_REPEAT 0x8370
#define GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS 0x8B4D

typedef void  (GL_APIENTRY * PFN_glBindVertexArray)(GLuint array);
static PFN_glBindVertexArray _sapp_glBindVertexArray;
typedef void  (GL_APIENTRY* PFN_glFramebufferTextureLayer)(GLenum target, GLenum attachment, GLuint texture, GLint level, GLint layer);
static PFN_glFramebufferTextureLayer _sapp_glFramebufferTextureLayer;
typedef void  (GL_APIENTRY* PFN_glGenFramebuffers)(GLsizei n, GLuint* framebuffers);
static PFN_glGenFramebuffers _sapp_glGenFramebuffers;
typedef void  (GL_APIENTRY* PFN_glBindFramebuffer)(GLenum target, GLuint framebuffer);
static PFN_glBindFramebuffer _sapp_glBindFramebuffer;
typedef void  (GL_APIENTRY* PFN_glBindRenderbuffer)(GLenum target, GLuint renderbuffer);
static PFN_glBindRenderbuffer _sapp_glBindRenderbuffer;
typedef const GLubyte* (GL_APIENTRY* PFN_glGetStringi)(GLenum name, GLuint index);
static PFN_glGetStringi _sapp_glGetStringi;
typedef void  (GL_APIENTRY* PFN_glClearBufferfi)(GLenum buffer, GLint drawbuffer, GLfloat depth, GLint stencil);
static PFN_glClearBufferfi _sapp_glClearBufferfi;
typedef void  (GL_APIENTRY* PFN_glClearBufferfv)(GLenum buffer, GLint drawbuffer, const GLfloat* value);
static PFN_glClearBufferfv _sapp_glClearBufferfv;
typedef void  (GL_APIENTRY* PFN_glClearBufferuiv)(GLenum buffer, GLint drawbuffer, const GLuint* value);
static PFN_glClearBufferuiv _sapp_glClearBufferuiv;
typedef void  (GL_APIENTRY* PFN_glDeleteRenderbuffers)(GLsizei n, const GLuint* renderbuffers);
static PFN_glDeleteRenderbuffers _sapp_glDeleteRenderbuffers;
typedef void  (GL_APIENTRY* PFN_glUniform4fv)(GLint location, GLsizei count, const GLfloat* value);
static PFN_glUniform4fv _sapp_glUniform4fv;
typedef void  (GL_APIENTRY* PFN_glUniform2fv)(GLint location, GLsizei count, const GLfloat* value);
static PFN_glUniform2fv _sapp_glUniform2fv;
typedef void  (GL_APIENTRY* PFN_glUseProgram)(GLuint program);
static PFN_glUseProgram _sapp_glUseProgram;
typedef void  (GL_APIENTRY* PFN_glShaderSource)(GLuint shader, GLsizei count, const GLchar* const* string, const GLint* length);
static PFN_glShaderSource _sapp_glShaderSource;
typedef void  (GL_APIENTRY* PFN_glLinkProgram)(GLuint program);
static PFN_glLinkProgram _sapp_glLinkProgram;
typedef GLint(GL_APIENTRY* PFN_glGetUniformLocation)(GLuint program, const GLchar* name);
static PFN_glGetUniformLocation _sapp_glGetUniformLocation;
typedef void  (GL_APIENTRY* PFN_glGetShaderiv)(GLuint shader, GLenum pname, GLint* params);
static PFN_glGetShaderiv _sapp_glGetShaderiv;
typedef void  (GL_APIENTRY* PFN_glGetProgramInfoLog)(GLuint program, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
static PFN_glGetProgramInfoLog _sapp_glGetProgramInfoLog;
typedef GLint(GL_APIENTRY* PFN_glGetAttribLocation)(GLuint program, const GLchar* name);
static PFN_glGetAttribLocation _sapp_glGetAttribLocation;
typedef void  (GL_APIENTRY* PFN_glDisableVertexAttribArray)(GLuint index);
static PFN_glDisableVertexAttribArray _sapp_glDisableVertexAttribArray;
typedef void  (GL_APIENTRY* PFN_glDeleteShader)(GLuint shader);
static PFN_glDeleteShader _sapp_glDeleteShader;
typedef void  (GL_APIENTRY* PFN_glDeleteProgram)(GLuint program);
static PFN_glDeleteProgram _sapp_glDeleteProgram;
typedef void  (GL_APIENTRY* PFN_glCompileShader)(GLuint shader);
static PFN_glCompileShader _sapp_glCompileShader;
typedef void  (GL_APIENTRY* PFN_glStencilFuncSeparate)(GLenum face, GLenum func, GLint ref, GLuint mask);
static PFN_glStencilFuncSeparate _sapp_glStencilFuncSeparate;
typedef void  (GL_APIENTRY* PFN_glStencilOpSeparate)(GLenum face, GLenum sfail, GLenum dpfail, GLenum dppass);
static PFN_glStencilOpSeparate _sapp_glStencilOpSeparate;
typedef void  (GL_APIENTRY* PFN_glRenderbufferStorageMultisample)(GLenum target, GLsizei samples, GLenum internalformat, GLsizei width, GLsizei height);
static PFN_glRenderbufferStorageMultisample _sapp_glRenderbufferStorageMultisample;
typedef void  (GL_APIENTRY* PFN_glDrawBuffers)(GLsizei n, const GLenum* bufs);
static PFN_glDrawBuffers _sapp_glDrawBuffers;
typedef void  (GL_APIENTRY* PFN_glVertexAttribDivisor)(GLuint index, GLuint divisor);
static PFN_glVertexAttribDivisor _sapp_glVertexAttribDivisor;
typedef void  (GL_APIENTRY* PFN_glBufferSubData)(GLenum target, GLintptr offset, GLsizeiptr size, const void* data);
static PFN_glBufferSubData _sapp_glBufferSubData;
typedef void  (GL_APIENTRY* PFN_glGenBuffers)(GLsizei n, GLuint* buffers);
static PFN_glGenBuffers _sapp_glGenBuffers;
typedef GLenum(GL_APIENTRY* PFN_glCheckFramebufferStatus)(GLenum target);
static PFN_glCheckFramebufferStatus _sapp_glCheckFramebufferStatus;
typedef void  (GL_APIENTRY* PFN_glFramebufferRenderbuffer)(GLenum target, GLenum attachment, GLenum renderbuffertarget, GLuint renderbuffer);
static PFN_glFramebufferRenderbuffer _sapp_glFramebufferRenderbuffer;
typedef void  (GL_APIENTRY* PFN_glCompressedTexImage2D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLint border, GLsizei imageSize, const void* data);
static PFN_glCompressedTexImage2D _sapp_glCompressedTexImage2D;
typedef void  (GL_APIENTRY* PFN_glCompressedTexImage3D)(GLenum target, GLint level, GLenum internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLsizei imageSize, const void* data);
static PFN_glCompressedTexImage3D _sapp_glCompressedTexImage3D;
typedef void  (GL_APIENTRY* PFN_glActiveTexture)(GLenum texture);
static PFN_glActiveTexture _sapp_glActiveTexture;
typedef void  (GL_APIENTRY* PFN_glTexSubImage3D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLint zoffset, GLsizei width, GLsizei height, GLsizei depth, GLenum format, GLenum type, const void* pixels);
static PFN_glTexSubImage3D _sapp_glTexSubImage3D;
typedef void  (GL_APIENTRY* PFN_glUniformMatrix4fv)(GLint location, GLsizei count, GLboolean transpose, const GLfloat* value);
static PFN_glUniformMatrix4fv _sapp_glUniformMatrix4fv;
typedef void  (GL_APIENTRY* PFN_glRenderbufferStorage)(GLenum target, GLenum internalformat, GLsizei width, GLsizei height);
static PFN_glRenderbufferStorage _sapp_glRenderbufferStorage;
typedef void  (GL_APIENTRY* PFN_glGenTextures)(GLsizei n, GLuint* textures);
static PFN_glGenTextures _sapp_glGenTextures;
typedef void  (GL_APIENTRY* PFN_glPolygonOffset)(GLfloat factor, GLfloat units);
static PFN_glPolygonOffset _sapp_glPolygonOffset;
typedef void  (GL_APIENTRY* PFN_glDrawElements)(GLenum mode, GLsizei count, GLenum type, const void* indices);
static PFN_glDrawElements _sapp_glDrawElements;
typedef void  (GL_APIENTRY* PFN_glDeleteFramebuffers)(GLsizei n, const GLuint* framebuffers);
static PFN_glDeleteFramebuffers _sapp_glDeleteFramebuffers;
typedef void  (GL_APIENTRY* PFN_glBlendEquationSeparate)(GLenum modeRGB, GLenum modeAlpha);
static PFN_glBlendEquationSeparate _sapp_glBlendEquationSeparate;
typedef void  (GL_APIENTRY* PFN_glDeleteTextures)(GLsizei n, const GLuint* textures);
static PFN_glDeleteTextures _sapp_glDeleteTextures;
typedef void  (GL_APIENTRY* PFN_glGetProgramiv)(GLuint program, GLenum pname, GLint* params);
static PFN_glGetProgramiv _sapp_glGetProgramiv;
typedef void  (GL_APIENTRY* PFN_glBindTexture)(GLenum target, GLuint texture);
static PFN_glBindTexture _sapp_glBindTexture;
typedef void  (GL_APIENTRY* PFN_glTexImage3D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLsizei depth, GLint border, GLenum format, GLenum type, const void* pixels);
static PFN_glTexImage3D _sapp_glTexImage3D;
typedef GLuint(GL_APIENTRY* PFN_glCreateShader)(GLenum type);
static PFN_glCreateShader _sapp_glCreateShader;
typedef void  (GL_APIENTRY* PFN_glTexSubImage2D)(GLenum target, GLint level, GLint xoffset, GLint yoffset, GLsizei width, GLsizei height, GLenum format, GLenum type, const void* pixels);
static PFN_glTexSubImage2D _sapp_glTexSubImage2D;
typedef void  (GL_APIENTRY* PFN_glClearDepth)(GLdouble depth);
static PFN_glClearDepth _sapp_glClearDepth;
typedef void  (GL_APIENTRY* PFN_glFramebufferTexture2D)(GLenum target, GLenum attachment, GLenum textarget, GLuint texture, GLint level);
static PFN_glFramebufferTexture2D _sapp_glFramebufferTexture2D;
typedef GLuint(GL_APIENTRY* PFN_glCreateProgram)();
static PFN_glCreateProgram _sapp_glCreateProgram;
typedef void  (GL_APIENTRY* PFN_glViewport)(GLint x, GLint y, GLsizei width, GLsizei height);
static PFN_glViewport _sapp_glViewport;
typedef void  (GL_APIENTRY* PFN_glDeleteBuffers)(GLsizei n, const GLuint* buffers);
static PFN_glDeleteBuffers _sapp_glDeleteBuffers;
typedef void  (GL_APIENTRY* PFN_glDrawArrays)(GLenum mode, GLint first, GLsizei count);
static PFN_glDrawArrays _sapp_glDrawArrays;
typedef void  (GL_APIENTRY* PFN_glDrawElementsInstanced)(GLenum mode, GLsizei count, GLenum type, const void* indices, GLsizei instancecount);
static PFN_glDrawElementsInstanced _sapp_glDrawElementsInstanced;
typedef void  (GL_APIENTRY* PFN_glVertexAttribPointer)(GLuint index, GLint size, GLenum type, GLboolean normalized, GLsizei stride, const void* pointer);
static PFN_glVertexAttribPointer _sapp_glVertexAttribPointer;
typedef void  (GL_APIENTRY* PFN_glUniform1i)(GLint location, GLint v0);
static PFN_glUniform1i _sapp_glUniform1i;
typedef void  (GL_APIENTRY* PFN_glDisable)(GLenum cap);
static PFN_glDisable _sapp_glDisable;
typedef void  (GL_APIENTRY* PFN_glColorMask)(GLboolean red, GLboolean green, GLboolean blue, GLboolean alpha);
static PFN_glColorMask _sapp_glColorMask;
typedef void  (GL_APIENTRY* PFN_glBindBuffer)(GLenum target, GLuint buffer);
static PFN_glBindBuffer _sapp_glBindBuffer;
typedef void  (GL_APIENTRY* PFN_glDeleteVertexArrays)(GLsizei n, const GLuint* arrays);
static PFN_glDeleteVertexArrays _sapp_glDeleteVertexArrays;
typedef void  (GL_APIENTRY* PFN_glDepthMask)(GLboolean flag);
static PFN_glDepthMask _sapp_glDepthMask;
typedef void  (GL_APIENTRY* PFN_glDrawArraysInstanced)(GLenum mode, GLint first, GLsizei count, GLsizei instancecount);
static PFN_glDrawArraysInstanced _sapp_glDrawArraysInstanced;
typedef void  (GL_APIENTRY* PFN_glClearStencil)(GLint s);
static PFN_glClearStencil _sapp_glClearStencil;
typedef void  (GL_APIENTRY* PFN_glScissor)(GLint x, GLint y, GLsizei width, GLsizei height);
static PFN_glScissor _sapp_glScissor;
typedef void  (GL_APIENTRY* PFN_glUniform3fv)(GLint location, GLsizei count, const GLfloat* value);
static PFN_glUniform3fv _sapp_glUniform3fv;
typedef void  (GL_APIENTRY* PFN_glGenRenderbuffers)(GLsizei n, GLuint* renderbuffers);
static PFN_glGenRenderbuffers _sapp_glGenRenderbuffers;
typedef void  (GL_APIENTRY* PFN_glBufferData)(GLenum target, GLsizeiptr size, const void* data, GLenum usage);
static PFN_glBufferData _sapp_glBufferData;
typedef void  (GL_APIENTRY* PFN_glBlendFuncSeparate)(GLenum sfactorRGB, GLenum dfactorRGB, GLenum sfactorAlpha, GLenum dfactorAlpha);
static PFN_glBlendFuncSeparate _sapp_glBlendFuncSeparate;
typedef void  (GL_APIENTRY* PFN_glTexParameteri)(GLenum target, GLenum pname, GLint param);
static PFN_glTexParameteri _sapp_glTexParameteri;
typedef void  (GL_APIENTRY* PFN_glGetIntegerv)(GLenum pname, GLint* data);
static PFN_glGetIntegerv _sapp_glGetIntegerv;
typedef void  (GL_APIENTRY* PFN_glEnable)(GLenum cap);
static PFN_glEnable _sapp_glEnable;
typedef void  (GL_APIENTRY* PFN_glBlitFramebuffer)(GLint srcX0, GLint srcY0, GLint srcX1, GLint srcY1, GLint dstX0, GLint dstY0, GLint dstX1, GLint dstY1, GLbitfield mask, GLenum filter);
static PFN_glBlitFramebuffer _sapp_glBlitFramebuffer;
typedef void  (GL_APIENTRY* PFN_glStencilMask)(GLuint mask);
static PFN_glStencilMask _sapp_glStencilMask;
typedef void  (GL_APIENTRY* PFN_glAttachShader)(GLuint program, GLuint shader);
static PFN_glAttachShader _sapp_glAttachShader;
typedef GLenum(GL_APIENTRY* PFN_glGetError)();
static PFN_glGetError _sapp_glGetError;
typedef void  (GL_APIENTRY* PFN_glClearColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static PFN_glClearColor _sapp_glClearColor;
typedef void  (GL_APIENTRY* PFN_glBlendColor)(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha);
static PFN_glBlendColor _sapp_glBlendColor;
typedef void  (GL_APIENTRY* PFN_glTexParameterf)(GLenum target, GLenum pname, GLfloat param);
static PFN_glTexParameterf _sapp_glTexParameterf;
typedef void  (GL_APIENTRY* PFN_glGetShaderInfoLog)(GLuint shader, GLsizei bufSize, GLsizei* length, GLchar* infoLog);
static PFN_glGetShaderInfoLog _sapp_glGetShaderInfoLog;
typedef void  (GL_APIENTRY* PFN_glDepthFunc)(GLenum func);
static PFN_glDepthFunc _sapp_glDepthFunc;
typedef void  (GL_APIENTRY* PFN_glStencilOp)(GLenum fail, GLenum zfail, GLenum zpass);
static PFN_glStencilOp _sapp_glStencilOp;
typedef void  (GL_APIENTRY* PFN_glStencilFunc)(GLenum func, GLint ref, GLuint mask);
static PFN_glStencilFunc _sapp_glStencilFunc;
typedef void  (GL_APIENTRY* PFN_glEnableVertexAttribArray)(GLuint index);
static PFN_glEnableVertexAttribArray _sapp_glEnableVertexAttribArray;
typedef void  (GL_APIENTRY* PFN_glBlendFunc)(GLenum sfactor, GLenum dfactor);
static PFN_glBlendFunc _sapp_glBlendFunc;
typedef void  (GL_APIENTRY* PFN_glUniform1fv)(GLint location, GLsizei count, const GLfloat* value);
static PFN_glUniform1fv _sapp_glUniform1fv;
typedef void  (GL_APIENTRY* PFN_glReadBuffer)(GLenum src);
static PFN_glReadBuffer _sapp_glReadBuffer;
typedef void  (GL_APIENTRY* PFN_glClear)(GLbitfield mask);
static PFN_glClear _sapp_glClear;
typedef void  (GL_APIENTRY* PFN_glTexImage2D)(GLenum target, GLint level, GLint internalformat, GLsizei width, GLsizei height, GLint border, GLenum format, GLenum type, const void* pixels);
static PFN_glTexImage2D _sapp_glTexImage2D;
typedef void  (GL_APIENTRY* PFN_glGenVertexArrays)(GLsizei n, GLuint* arrays);
static PFN_glGenVertexArrays _sapp_glGenVertexArrays;
typedef void  (GL_APIENTRY* PFN_glFrontFace)(GLenum mode);
static PFN_glFrontFace _sapp_glFrontFace;
typedef void  (GL_APIENTRY* PFN_glCullFace)(GLenum mode);
static PFN_glCullFace _sapp_glCullFace;

void* _sapp_win32_glgetprocaddr(const char* name) {
	void* proc_addr = (void*)_sapp_wglGetProcAddress(name);
	if (0 == proc_addr) {
		proc_addr = (void*)GetProcAddress(_sapp_opengl32, name);
	}
	a_assert(proc_addr);
	return proc_addr;
}

#define _SAPP_GLPROC(name) _sapp_ ## name = (PFN_ ## name) _sapp_win32_glgetprocaddr(#name)

void _sapp_win32_gl_loadfuncs(void) {
	a_assert(_sapp_wglGetProcAddress);
	a_assert(_sapp_opengl32);
	_SAPP_GLPROC(glBindVertexArray);
	_SAPP_GLPROC(glFramebufferTextureLayer);
	_SAPP_GLPROC(glGenFramebuffers);
	_SAPP_GLPROC(glBindFramebuffer);
	_SAPP_GLPROC(glBindRenderbuffer);
	_SAPP_GLPROC(glGetStringi);
	_SAPP_GLPROC(glClearBufferfi);
	_SAPP_GLPROC(glClearBufferfv);
	_SAPP_GLPROC(glClearBufferuiv);
	_SAPP_GLPROC(glDeleteRenderbuffers);
	_SAPP_GLPROC(glUniform4fv);
	_SAPP_GLPROC(glUniform2fv);
	_SAPP_GLPROC(glUseProgram);
	_SAPP_GLPROC(glShaderSource);
	_SAPP_GLPROC(glLinkProgram);
	_SAPP_GLPROC(glGetUniformLocation);
	_SAPP_GLPROC(glGetShaderiv);
	_SAPP_GLPROC(glGetProgramInfoLog);
	_SAPP_GLPROC(glGetAttribLocation);
	_SAPP_GLPROC(glDisableVertexAttribArray);
	_SAPP_GLPROC(glDeleteShader);
	_SAPP_GLPROC(glDeleteProgram);
	_SAPP_GLPROC(glCompileShader);
	_SAPP_GLPROC(glStencilFuncSeparate);
	_SAPP_GLPROC(glStencilOpSeparate);
	_SAPP_GLPROC(glRenderbufferStorageMultisample);
	_SAPP_GLPROC(glDrawBuffers);
	_SAPP_GLPROC(glVertexAttribDivisor);
	_SAPP_GLPROC(glBufferSubData);
	_SAPP_GLPROC(glGenBuffers);
	_SAPP_GLPROC(glCheckFramebufferStatus);
	_SAPP_GLPROC(glFramebufferRenderbuffer);
	_SAPP_GLPROC(glCompressedTexImage2D);
	_SAPP_GLPROC(glCompressedTexImage3D);
	_SAPP_GLPROC(glActiveTexture);
	_SAPP_GLPROC(glTexSubImage3D);
	_SAPP_GLPROC(glUniformMatrix4fv);
	_SAPP_GLPROC(glRenderbufferStorage);
	_SAPP_GLPROC(glGenTextures);
	_SAPP_GLPROC(glPolygonOffset);
	_SAPP_GLPROC(glDrawElements);
	_SAPP_GLPROC(glDeleteFramebuffers);
	_SAPP_GLPROC(glBlendEquationSeparate);
	_SAPP_GLPROC(glDeleteTextures);
	_SAPP_GLPROC(glGetProgramiv);
	_SAPP_GLPROC(glBindTexture);
	_SAPP_GLPROC(glTexImage3D);
	_SAPP_GLPROC(glCreateShader);
	_SAPP_GLPROC(glTexSubImage2D);
	_SAPP_GLPROC(glClearDepth);
	_SAPP_GLPROC(glFramebufferTexture2D);
	_SAPP_GLPROC(glCreateProgram);
	_SAPP_GLPROC(glViewport);
	_SAPP_GLPROC(glDeleteBuffers);
	_SAPP_GLPROC(glDrawArrays);
	_SAPP_GLPROC(glDrawElementsInstanced);
	_SAPP_GLPROC(glVertexAttribPointer);
	_SAPP_GLPROC(glUniform1i);
	_SAPP_GLPROC(glDisable);
	_SAPP_GLPROC(glColorMask);
	_SAPP_GLPROC(glBindBuffer);
	_SAPP_GLPROC(glDeleteVertexArrays);
	_SAPP_GLPROC(glDepthMask);
	_SAPP_GLPROC(glDrawArraysInstanced);
	_SAPP_GLPROC(glClearStencil);
	_SAPP_GLPROC(glScissor);
	_SAPP_GLPROC(glUniform3fv);
	_SAPP_GLPROC(glGenRenderbuffers);
	_SAPP_GLPROC(glBufferData);
	_SAPP_GLPROC(glBlendFuncSeparate);
	_SAPP_GLPROC(glTexParameteri);
	_SAPP_GLPROC(glGetIntegerv);
	_SAPP_GLPROC(glEnable);
	_SAPP_GLPROC(glBlitFramebuffer);
	_SAPP_GLPROC(glStencilMask);
	_SAPP_GLPROC(glAttachShader);
	_SAPP_GLPROC(glGetError);
	_SAPP_GLPROC(glClearColor);
	_SAPP_GLPROC(glBlendColor);
	_SAPP_GLPROC(glTexParameterf);
	_SAPP_GLPROC(glGetShaderInfoLog);
	_SAPP_GLPROC(glDepthFunc);
	_SAPP_GLPROC(glStencilOp);
	_SAPP_GLPROC(glStencilFunc);
	_SAPP_GLPROC(glEnableVertexAttribArray);
	_SAPP_GLPROC(glBlendFunc);
	_SAPP_GLPROC(glUniform1fv);
	_SAPP_GLPROC(glReadBuffer);
	_SAPP_GLPROC(glClear);
	_SAPP_GLPROC(glTexImage2D);
	_SAPP_GLPROC(glGenVertexArrays);
	_SAPP_GLPROC(glFrontFace);
	_SAPP_GLPROC(glCullFace);
}
#define glBindVertexArray _sapp_glBindVertexArray
#define glFramebufferTextureLayer _sapp_glFramebufferTextureLayer
#define glGenFramebuffers _sapp_glGenFramebuffers
#define glBindFramebuffer _sapp_glBindFramebuffer
#define glBindRenderbuffer _sapp_glBindRenderbuffer
#define glGetStringi _sapp_glGetStringi
#define glClearBufferfi _sapp_glClearBufferfi
#define glClearBufferfv _sapp_glClearBufferfv
#define glClearBufferuiv _sapp_glClearBufferuiv
#define glDeleteRenderbuffers _sapp_glDeleteRenderbuffers
#define glUniform4fv _sapp_glUniform4fv
#define glUniform2fv _sapp_glUniform2fv
#define glUseProgram _sapp_glUseProgram
#define glShaderSource _sapp_glShaderSource
#define glLinkProgram _sapp_glLinkProgram
#define glGetUniformLocation _sapp_glGetUniformLocation
#define glGetShaderiv _sapp_glGetShaderiv
#define glGetProgramInfoLog _sapp_glGetProgramInfoLog
#define glGetAttribLocation _sapp_glGetAttribLocation
#define glDisableVertexAttribArray _sapp_glDisableVertexAttribArray
#define glDeleteShader _sapp_glDeleteShader
#define glDeleteProgram _sapp_glDeleteProgram
#define glCompileShader _sapp_glCompileShader
#define glStencilFuncSeparate _sapp_glStencilFuncSeparate
#define glStencilOpSeparate _sapp_glStencilOpSeparate
#define glRenderbufferStorageMultisample _sapp_glRenderbufferStorageMultisample
#define glDrawBuffers _sapp_glDrawBuffers
#define glVertexAttribDivisor _sapp_glVertexAttribDivisor
#define glBufferSubData _sapp_glBufferSubData
#define glGenBuffers _sapp_glGenBuffers
#define glCheckFramebufferStatus _sapp_glCheckFramebufferStatus
#define glFramebufferRenderbuffer _sapp_glFramebufferRenderbuffer
#define glCompressedTexImage2D _sapp_glCompressedTexImage2D
#define glCompressedTexImage3D _sapp_glCompressedTexImage3D
#define glActiveTexture _sapp_glActiveTexture
#define glTexSubImage3D _sapp_glTexSubImage3D
#define glUniformMatrix4fv _sapp_glUniformMatrix4fv
#define glRenderbufferStorage _sapp_glRenderbufferStorage
#define glGenTextures _sapp_glGenTextures
#define glPolygonOffset _sapp_glPolygonOffset
#define glDrawElements _sapp_glDrawElements
#define glDeleteFramebuffers _sapp_glDeleteFramebuffers
#define glBlendEquationSeparate _sapp_glBlendEquationSeparate
#define glDeleteTextures _sapp_glDeleteTextures
#define glGetProgramiv _sapp_glGetProgramiv
#define glBindTexture _sapp_glBindTexture
#define glTexImage3D _sapp_glTexImage3D
#define glCreateShader _sapp_glCreateShader
#define glTexSubImage2D _sapp_glTexSubImage2D
#define glClearDepth _sapp_glClearDepth
#define glFramebufferTexture2D _sapp_glFramebufferTexture2D
#define glCreateProgram _sapp_glCreateProgram
#define glViewport _sapp_glViewport
#define glDeleteBuffers _sapp_glDeleteBuffers
#define glDrawArrays _sapp_glDrawArrays
#define glDrawElementsInstanced _sapp_glDrawElementsInstanced
#define glVertexAttribPointer _sapp_glVertexAttribPointer
#define glUniform1i _sapp_glUniform1i
#define glDisable _sapp_glDisable
#define glColorMask _sapp_glColorMask
#define glBindBuffer _sapp_glBindBuffer
#define glDeleteVertexArrays _sapp_glDeleteVertexArrays
#define glDepthMask _sapp_glDepthMask
#define glDrawArraysInstanced _sapp_glDrawArraysInstanced
#define glClearStencil _sapp_glClearStencil
#define glScissor _sapp_glScissor
#define glUniform3fv _sapp_glUniform3fv
#define glGenRenderbuffers _sapp_glGenRenderbuffers
#define glBufferData _sapp_glBufferData
#define glBlendFuncSeparate _sapp_glBlendFuncSeparate
#define glTexParameteri _sapp_glTexParameteri
#define glGetIntegerv _sapp_glGetIntegerv
#define glEnable _sapp_glEnable
#define glBlitFramebuffer _sapp_glBlitFramebuffer
#define glStencilMask _sapp_glStencilMask
#define glAttachShader _sapp_glAttachShader
#define glGetError _sapp_glGetError
#define glClearColor _sapp_glClearColor
#define glBlendColor _sapp_glBlendColor
#define glTexParameterf _sapp_glTexParameterf
#define glGetShaderInfoLog _sapp_glGetShaderInfoLog
#define glDepthFunc _sapp_glDepthFunc
#define glStencilOp _sapp_glStencilOp
#define glStencilFunc _sapp_glStencilFunc
#define glEnableVertexAttribArray _sapp_glEnableVertexAttribArray
#define glBlendFunc _sapp_glBlendFunc
#define glUniform1fv _sapp_glUniform1fv
#define glReadBuffer _sapp_glReadBuffer
#define glClear _sapp_glClear
#define glTexImage2D _sapp_glTexImage2D
#define glGenVertexArrays _sapp_glGenVertexArrays
#define glFrontFace _sapp_glFrontFace
#define glCullFace _sapp_glCullFace

#endif /* SOKOL_WIN32_NO_GL_LOADER */

#define SOKOL_IMPL
//#define SOKOL_GLCORE33
#include "sokol_gfx.h"

namespace ari
{
    namespace gfx
    {
		bool ari::gfx::SetupGfx(gfxSetup& setup)
		{
			auto window = io::CreateAriWindow(setup.window, "Ari 0.1");
			if (!window.IsValid())
				return false;

			_sapp_wgl_init();
			_sapp_wgl_load_extensions();
			_sapp_wgl_create_context(window, setup);
#if !defined(SOKOL_WIN32_NO_GL_LOADER)
			_sapp_win32_gl_loadfuncs();
#endif

			// Setup sokol gfx
			sg_desc desc;
			core::Memory::Fill(&desc, sizeof(sg_desc), 0);
			sg_setup(&desc);

			return true;
		}

		void Present()
		{
			const io::WindowHandle window{ 0 , 0 };
			Present(window);
		}

		void Present(const io::WindowHandle& handle)
		{
			_sapp_wgl_swap_buffers(handle);
		}		

    } // namespace gfx
    
} // namespace ari
