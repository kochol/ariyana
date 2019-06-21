#include "gfx_ogl.hpp"
#include "core/Memory.hpp"

void _sapp_gl_init_fbconfig(_sapp_gl_fbconfig* fbconfig) {
	ari::core::Memory::Fill(fbconfig, sizeof(_sapp_gl_fbconfig), 0);
	/* -1 means "don't care" */
	fbconfig->red_bits = -1;
	fbconfig->green_bits = -1;
	fbconfig->blue_bits = -1;
	fbconfig->alpha_bits = -1;
	fbconfig->depth_bits = -1;
	fbconfig->stencil_bits = -1;
	fbconfig->samples = -1;
}

const _sapp_gl_fbconfig* _sapp_gl_choose_fbconfig(const _sapp_gl_fbconfig* desired, const _sapp_gl_fbconfig* alternatives, unsigned int count) {
	unsigned int i;
	unsigned int missing, least_missing = 1000000;
	unsigned int color_diff, least_color_diff = 10000000;
	unsigned int extra_diff, least_extra_diff = 10000000;
	const _sapp_gl_fbconfig* current;
	const _sapp_gl_fbconfig* closest = NULL;
	for (i = 0; i < count; i++) {
		current = alternatives + i;
		if (desired->doublebuffer != current->doublebuffer) {
			continue;
		}
		missing = 0;
		if (desired->alpha_bits > 0 && current->alpha_bits == 0) {
			missing++;
		}
		if (desired->depth_bits > 0 && current->depth_bits == 0) {
			missing++;
		}
		if (desired->stencil_bits > 0 && current->stencil_bits == 0) {
			missing++;
		}
		if (desired->samples > 0 && current->samples == 0) {
			/* Technically, several multisampling buffers could be
				involved, but that's a lower level implementation detail and
				not important to us here, so we count them as one
			*/
			missing++;
		}

		/* These polynomials make many small channel size differences matter
			less than one large channel size difference
			Calculate color channel size difference value
		*/
		color_diff = 0;
		if (desired->red_bits != -1) {
			color_diff += (desired->red_bits - current->red_bits) * (desired->red_bits - current->red_bits);
		}
		if (desired->green_bits != -1) {
			color_diff += (desired->green_bits - current->green_bits) * (desired->green_bits - current->green_bits);
		}
		if (desired->blue_bits != -1) {
			color_diff += (desired->blue_bits - current->blue_bits) * (desired->blue_bits - current->blue_bits);
		}

		/* Calculate non-color channel size difference value */
		extra_diff = 0;
		if (desired->alpha_bits != -1) {
			extra_diff += (desired->alpha_bits - current->alpha_bits) * (desired->alpha_bits - current->alpha_bits);
		}
		if (desired->depth_bits != -1) {
			extra_diff += (desired->depth_bits - current->depth_bits) * (desired->depth_bits - current->depth_bits);
		}
		if (desired->stencil_bits != -1) {
			extra_diff += (desired->stencil_bits - current->stencil_bits) * (desired->stencil_bits - current->stencil_bits);
		}
		if (desired->samples != -1) {
			extra_diff += (desired->samples - current->samples) * (desired->samples - current->samples);
		}

		/* Figure out if the current one is better than the best one found so far
			Least number of missing buffers is the most important heuristic,
			then color buffer size match and lastly size match for other buffers
		*/
		if (missing < least_missing) {
			closest = current;
		}
		else if (missing == least_missing) {
			if ((color_diff < least_color_diff) ||
				(color_diff == least_color_diff && extra_diff < least_extra_diff))
			{
				closest = current;
			}
		}
		if (current == closest) {
			least_missing = missing;
			least_color_diff = color_diff;
			least_extra_diff = extra_diff;
		}
	}
	return closest;
}