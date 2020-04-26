@module ari

@include ../shared/ctypes.glsl

@vs vs

uniform vs_params {
    mat4 mvp;
};

in vec4 pos;
void main() {
    gl_Position = mvp * pos;
}
@end

@fs fs
out vec4 frag_color;
void main() {
    frag_color = vec4(1,1,1,1);
}
@end

@program basic vs fs

@vs vs_tex

uniform vs_params 
{
    mat4 mvp;
};

in vec4 pos;
in vec3 normal;
in vec2 texcoord0;
out vec2 uv;
void main() 
{
    gl_Position = mvp * pos;
	uv = texcoord0;
}

@end

@fs fs_tex
uniform sampler2D tex;

in vec2 uv;
out vec4 frag_color;
void main() {
    frag_color = texture(tex, uv);
}
@end

@program basic_tex vs_tex fs_tex
