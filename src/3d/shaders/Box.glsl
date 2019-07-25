@ctype mat4 sx_mat4

@vs vs

uniform vs_params {
    mat4 mvp;
};

in vec4 pos;
in vec4 color0;
out vec4 color;
void main() {
    gl_Position = mvp * pos;
	color = color0;
}
@end

@fs fs
in vec4 color;
out vec4 frag_color;
void main() {
    frag_color = color;
}
@end

@program box vs fs

@vs vs_tex
uniform vs_params {
    mat4 mvp;
};

in vec4 pos;
in vec2 texcoord0;
out vec2 uv;
void main() {
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

@program box_tex vs_tex fs_tex
