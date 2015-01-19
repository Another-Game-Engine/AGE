#pragma once

#include <map>
#include <set>

#include <Components/Component.hh>
#include <Render/Program.hh>
#include <cereal/types/string.hpp>
#include <Entities/Entity.hh>
#include <Core/AScene.hh>
#include <Core/PrepareKey.hpp>
#include <Skinning/AnimationInstance.hpp>
#include <cereal/types/memory.hpp>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>

namespace AGE
{
	struct MeshInstance;
}

namespace Component
{
	struct MeshRenderer : public Component::ComponentBase<MeshRenderer>
	{
		MeshRenderer();
		virtual ~MeshRenderer();
		MeshRenderer(MeshRenderer &&o);
		MeshRenderer &operator=(MeshRenderer &&o);
		
		void init(AScene *, std::shared_ptr<AGE::MeshInstance> file);
		virtual void reset(AScene *);

		template <typename Archive> void save(Archive &ar) const;
		template <typename Archive> void load(Archive &ar);


		MeshRenderer &setMesh(const std::shared_ptr<AGE::MeshInstance> &_mesh);
		std::shared_ptr<MeshInstance> getMesh();
		MeshRenderer &setMaterial(const std::shared_ptr<MaterialSetInstance> &_mesh);
		std::shared_ptr<MaterialSetInstance> getMaterial();
		MeshRenderer &setAnimation(const Key<AGE::AnimationInstance> &key);

		virtual void postUnserialization(AScene *scene);

	private:
		AGE::PrepareKey _key;
		AScene *_scene;
		std::shared_ptr<MeshInstance> _mesh;
		std::shared_ptr<MaterialSetInstance> _material;
		Key<AGE::AnimationInstance> _animation;

		struct SerializationInfos
		{
			std::string mesh;
			std::string material;
			std::string animation;
			template < typename Archive >
			void serialize(Archive &ar)
			{
				ar(mesh, material, animation);
			}
		};

		std::unique_ptr<SerializationInfos> _serializationInfos;

		void updateGeometry();
		MeshRenderer(MeshRenderer const &) = delete;
		MeshRenderer &operator=(MeshRenderer const &) = delete;
	};

	template <typename Archive>
	void MeshRenderer::save(Archive &ar) const
	{
		auto serializationInfos = std::make_unique<SerializationInfos>();
		if (_material)
		{
			serializationInfos->material = _material->path;
		}
		if (_mesh)
		{
			serializationInfos->mesh = _mesh->path;
		}
		//todo with animations
		ar(serializationInfos);
	}

	template <typename Archive>
	void MeshRenderer::load(Archive &ar)
	{
		ar(_serializationInfos);
	}
}
