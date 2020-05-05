@block vs_main

in vec4 pos;
#if HAS_TEXCOORD
in vec2 texcoord0;
out vec2 uv;
#endif
#if HAS_VERTEXCOLOR
in vec4 color0;
out vec4 color;
#endif
#if HAS_NORMAL
in vec3 normal0;
out vec3 FragPos;
out vec3 Normal;
#endif

void main()
{
    gl_Position = mvp * pos;
#if HAS_TEXCOORD
    uv = texcoord0;
#endif
#if HAS_VERTEXCOLOR
    color = color0;
#endif
#if HAS_NORMAL
    FragPos = vec3(matWorld * pos);
    Normal = mat3(matWorld) * normal0;
#endif
}
@end
