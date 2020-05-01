@module ari

@include ../shared/ctypes.glsl

// basic withe shader

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

// basic textured shader
@vs vs_tex

uniform vs_params 
{
    mat4 mvp;
};

layout(location=0) in vec4 position;
layout(location=1) in vec2 texcoord0;
//layout(location=1) in vec3 normal;
 
out vec2 uv;

void main() 
{
    gl_Position = mvp * position;
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

// basic colored shader

// basic vertex color shader

@vs vs_vertex_color

uniform vs_params {
    mat4 mvp;
};

in vec4 pos;
layout(location=2)in vec4 color0;
out vec4 color;

void main() 
{ 
    gl_Position = mvp * pos;
    color = color0;
}
@end

@fs fs_vertex_color
in vec4 color;
out vec4 frag_color;

void main() 
{
    frag_color = color;
}
@end

@program basic_vertex_color vs_vertex_color fs_vertex_color
