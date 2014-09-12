#pragma once

#include <map>
#include <set>

#include <Components/Component.hh>
#include <Render/Shader.hh>
#include <cereal/types/string.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <Core/PrepareElement.hh>

namespace Resources
{
	class Texture;
};

enum Shadow;
class Material;
class Renderer;


namespace Component
{

	struct MeshRenderer : public Component::ComponentBase<MeshRenderer>, public AGE::PrepareElement
	{
		MeshRenderer();
		virtual ~MeshRenderer();
		MeshRenderer(MeshRenderer &&o);
		MeshRenderer &operator=(MeshRenderer &&o);
		void init(AScene *, std::shared_ptr<AGE::MeshInstance> file);
		virtual void reset(AScene *);

		template <typename Archive> void save(Archive &ar) const;
		template <typename Archive> void load(Archive &ar);

		MeshRenderer &updateOctree();
		virtual PrepareElement &initOctree(::AScene *scene, ENTITY_ID entityId);
		virtual PrepareElement &resetOctree(::AScene *scene, ENTITY_ID entityId);

		MeshRenderer &setMesh(const std::shared_ptr<AGE::MeshInstance> &_mesh);
		std::shared_ptr<AGE::MeshInstance> getMesh();
		MeshRenderer &setMaterial(const std::shared_ptr<AGE::MaterialSetInstance> &_mesh);
		std::shared_ptr<AGE::MaterialSetInstance> getMaterial();

	private:
		std::shared_ptr<AGE::MeshInstance> mesh;
		std::shared_ptr<AGE::MaterialSetInstance> material;
	
		MeshRenderer(MeshRenderer const &) = delete;
		MeshRenderer	&operator=(MeshRenderer const &) = delete;
	};

	template <typename Archive>
	void MeshRenderer::save(Archive &ar) const
	{

	}

	template <typename Archive>
	void MeshRenderer::load(Archive &ar)
	{

	}
}
