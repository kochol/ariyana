#pragma once
#include <cstdint>

typedef struct {
	int         red_bits;
	int         green_bits;
	int         blue_bits;
	int         alpha_bits;
	int         depth_bits;
	int         stencil_bits;
	int         samples;
	bool        doublebuffer;
	uintptr_t   handle;
} _sapp_gl_fbconfig;

void _sapp_gl_init_fbconfig(_sapp_gl_fbconfig* fbconfig);

const _sapp_gl_fbconfig* _sapp_gl_choose_fbconfig(const _sapp_gl_fbconfig* desired, const _sapp_gl_fbconfig* alternatives, unsigned int count);
