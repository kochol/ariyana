@block fs_main

out vec4 frag_color;

#if HAS_TEXCOORD
uniform sampler2D tex;
in vec2 uv;
#endif
#if HAS_VERTEXCOLOR
in vec4 color;
#endif
#if HAS_NORMAL
in vec3 FragPos;
in vec3 Normal;
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

#if HAS_NORMAL
    vec3 norm = normalize(Normal);
    float diff = 0.0;

#   if HAS_DIR_LIGHT
    diff += max(dot(norm, lightDir), 0.0);
#   endif
#   if HAS_POINT_LIGHT
    vec3 lightDir = normalize(lightPos - FragPos);
    diff += max(dot(norm, lightDir), 0.0);
#   endif    
   
    vec3 viewDir = normalize(camPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    float specular = specularStrength * spec;

    frag_color = frag_color * 0.1 + frag_color * vec4((diff + specular ) * lightColor, 1.0);
#endif
}

@end