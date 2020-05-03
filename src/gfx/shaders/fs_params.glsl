@block fs_params
#if HAS_NORMAL
    vec3 camPos;
    float specularStrength;
#endif
#if HAS_DIR_LIGHT
    vec3 lightDir;
    vec3 lightColor;
#endif
#if HAS_POINT_LIGHT
    vec3 lightPos;
    vec3 lightColor;
#endif
@end