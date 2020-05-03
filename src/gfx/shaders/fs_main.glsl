@block fs_main

out vec4 frag_color;

#if HAS_TEXCOORD
uniform sampler2D tex;
in vec2 uv;
#endif
#if HAS_VERTEXCOLOR
in vec4 color;
#endif

void main() 
{
#if HAS_TEXCOORD
    frag_color = texture(tex, uv);
#elif !HAS_VERTEXCOLOR
    frag_color = vec4(1.0);
#endif

#if HAS_VERTEXCOLOR
    frag_color += color;
#endif
}

@end