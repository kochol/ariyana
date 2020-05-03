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
@include_block fs_params
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
@include_block fs_params
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
@include_block fs_params
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
