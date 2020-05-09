@module ari

@include shared/ctypes.glsl
@include vs_params.glsl
@include vs_main.glsl
@include fs_params.glsl
@include fs_main.glsl

//================================================================================
// vs_mesh_
@vs vs_mesh_
uniform vs_params_mesh_
{
@include_block vs_params
}; 
@include_block vs_main
@end 

// fs_mesh_
@fs fs_mesh_
uniform fs_params_mesh_
{
@include_block fs_params
};
@include_block fs_main
@end
 
// mesh_
@program mesh_ vs_mesh_ fs_mesh_

//================================================================================
// vs_mesh_T
@vs vs_mesh_T
#define HAS_TEXCOORD 1
uniform vs_params_mesh_T
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_T
@fs fs_mesh_T
#define HAS_TEXCOORD 1
uniform fs_params_mesh_T
{
@include_block fs_params
};
@include_block fs_main
@end

// mesh_T
@program mesh_T vs_mesh_T fs_mesh_T

//================================================================================
// vs_mesh_VC
@vs vs_mesh_VC
#define HAS_VERTEXCOLOR 1
uniform vs_params_mesh_VC
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_VC
@fs fs_mesh_VC
#define HAS_VERTEXCOLOR 1
uniform fs_params_mesh_VC
{
@include_block fs_params
};
@include_block fs_main
@end

// mesh_VC
@program mesh_VC vs_mesh_VC fs_mesh_VC

//================================================================================
// vs_mesh_TND
@vs vs_mesh_TND
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
uniform vs_params_mesh_TND
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_TND
@fs fs_mesh_TND
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
uniform fs_params_mesh_TND
{
@include_block fs_params
};
@include_block fs_main
@end
 
// mesh_TND
@program mesh_TND vs_mesh_TND fs_mesh_TND

//================================================================================
// vs_mesh_TNP
@vs vs_mesh_TNP
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
uniform vs_params_mesh_TNP
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_TNP
@fs fs_mesh_TNP
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
uniform fs_params_mesh_TNP
{
@include_block fs_params
};
@include_block fs_main 
@end 

// mesh_TNP
@program mesh_TNP vs_mesh_TNP fs_mesh_TNP

//================================================================================
// vs_mesh_TNMD
@vs vs_mesh_TNMD
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
#define HAS_SHADOW_AO_S_MAP 1
uniform vs_params_mesh_TNMD
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_TNMD
@fs fs_mesh_TNMD
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
#define HAS_SHADOW_AO_S_MAP 1
uniform fs_params_mesh_TNMD
{
@include_block fs_params
};
@include_block fs_main
@end
 
// mesh_TNMD
@program mesh_TNMD vs_mesh_TNMD fs_mesh_TNMD

//================================================================================
// vs_mesh_TNMP
@vs vs_mesh_TNMP
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
#define HAS_SHADOW_AO_S_MAP 1
uniform vs_params_mesh_TNMP
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_TNMP
@fs fs_mesh_TNMP
#define HAS_TEXCOORD 1
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
#define HAS_SHADOW_AO_S_MAP 1
uniform fs_params_mesh_TNMP 
{
@include_block fs_params
};
@include_block fs_main
@end

// mesh_TNMP
@program mesh_TNMP vs_mesh_TNMP fs_mesh_TNMP

//================================================================================
// vs_mesh_VCND
@vs vs_mesh_VCND
#define HAS_VERTEXCOLOR 1
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
uniform vs_params_mesh_VCND
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_VCND
@fs fs_mesh_VCND
#define HAS_VERTEXCOLOR 1
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
uniform fs_params_mesh_VCND
{
@include_block fs_params
};
@include_block fs_main
@end
 
// mesh_VCND
@program mesh_VCND vs_mesh_VCND fs_mesh_VCND

//================================================================================
// vs_mesh_VCNP
@vs vs_mesh_VCNP
#define HAS_VERTEXCOLOR 1
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
uniform vs_params_mesh_VCNP
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_VCNP
@fs fs_mesh_VCNP
#define HAS_VERTEXCOLOR 1
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
uniform fs_params_mesh_VCNP
{
@include_block fs_params
};
@include_block fs_main
@end

// mesh_VCNP
@program mesh_VCNP vs_mesh_VCNP fs_mesh_VCNP

//================================================================================
// vs_mesh_ND
@vs vs_mesh_ND
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
uniform vs_params_mesh_ND
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_ND
@fs fs_mesh_ND
#define HAS_NORMAL 1
#define HAS_DIR_LIGHT 1
uniform fs_params_mesh_ND
{
@include_block fs_params
};
@include_block fs_main
@end
 
// mesh_ND
@program mesh_ND vs_mesh_ND fs_mesh_ND

//================================================================================
// vs_mesh_NP
@vs vs_mesh_NP
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
uniform vs_params_mesh_NP
{
@include_block vs_params
};
@include_block vs_main
@end

// fs_mesh_NP
@fs fs_mesh_NP
#define HAS_NORMAL 1
#define HAS_POINT_LIGHT 1
uniform fs_params_mesh_NP
{
@include_block fs_params
};
@include_block fs_main
@end

// mesh_NP
@program mesh_NP vs_mesh_NP fs_mesh_NP
