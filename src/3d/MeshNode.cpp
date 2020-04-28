#include "MeshNode.hpp"
#include "en/ComponentManager.hpp"

namespace ari::en
{
    ARI_COMPONENT_IMP(MeshNode);

    void MeshNode::Render(const int& _frameTurnIndex)
    {
	    if (Mesh.IsValid())
	    {
			sx_mat4 mvp = gfx::GetViewProjMatrix() * _finalMat[_frameTurnIndex];

			auto mesh = core::ObjectPool<gfx::Mesh>::GetByIndex(Mesh.Index);
			for (auto sub_mesh_hdl: mesh->SubMeshes)
			{
				if (sub_mesh_hdl.IsValid())
				{
					const auto sub_mesh = core::ObjectPool<gfx::SubMesh>::GetByIndex(sub_mesh_hdl.Index);
					gfx::ApplyPipeline(sub_mesh->Pipeline);
					gfx::ApplyBindings(sub_mesh->Binding);
					ApplyUniforms(gfx::ShaderStage::VertexShader, 0, &mvp, sizeof(sx_mat4));
					gfx::Draw(0, sub_mesh->ElementsCount, 1);
				}
			}
	    }
    }

} // namespace ari::en
