@module ari

@include ../../gfx/shaders/shared/ctypes.glsl

@vs vs

uniform vs_params {
    mat4 mvp;
	vec4 colorf;
};

in vec4 pos;
in vec2 uv0;
out vec2 uv;
out vec4 color0;

void main() {
    gl_Position = mvp * vec4(pos.xy, 0.0f, 1.0f);
    uv = uv0;
	color0 = colorf;
}
@end

@fs fs

in vec2 uv;
in vec4 color0;
out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = color0 * texture(tex, uv);
}
@end

@program sprite vs fs
