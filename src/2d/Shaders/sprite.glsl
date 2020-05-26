@module ari

@include ../../gfx/shaders/shared/ctypes.glsl

@vs vs

uniform vs_params {
    mat4 mvp;
};

in vec4 pos;
in vec2 uv0;
out vec2 uv;

void main() {
    gl_Position = mvp * vec4(pos.xy, 0.0f, 1.0f);
    uv = uv0;
}
@end

@fs fs

uniform fs_params {
	vec4 color;
};

in vec2 uv;
out vec4 frag_color;

uniform sampler2D tex;

void main() {
    frag_color = color * texture(tex, uv);
}
@end

@program sprite vs fs
