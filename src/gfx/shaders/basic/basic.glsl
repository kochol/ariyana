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

@program box vs fs
