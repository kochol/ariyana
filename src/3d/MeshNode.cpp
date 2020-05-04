#include "MeshNode.hpp"
#include "en/ComponentManager.hpp"

namespace ari::en
{
    ARI_COMPONENT_IMP(MeshNode);

    void MeshNode::Render(const int& _frameTurnIndex)
    {
	    if (Mesh.IsValid())
	    {
			gfx::SetWorldMatrix(_finalMat[_frameTurnIndex]);
			gfx::SetNormalMatrix(_normalMat[_frameTurnIndex]);

			auto mesh = core::ObjectPool<gfx::Mesh>::GetByIndex(Mesh.Index);
			for (auto sub_mesh_hdl: mesh->SubMeshes)
			{
				if (sub_mesh_hdl.IsValid())
				{
					const auto sub_mesh = core::ObjectPool<gfx::SubMesh>::GetByIndex(sub_mesh_hdl.Index);
					gfx::SetMaterialShader(sub_mesh->Material);
					gfx::ApplyPipelineAndMaterial(sub_mesh->Pipeline, &sub_mesh->Material);
					gfx::ApplyBindings(sub_mesh->Binding);
					gfx::Draw(0, sub_mesh->ElementsCount, 1);
				}
			}
	    }
    }

} // namespace ari::en
