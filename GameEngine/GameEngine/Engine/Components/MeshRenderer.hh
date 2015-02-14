#pragma once

#include <Components/Component.hh>
#include <cereal/types/string.hpp>
#include <Entities/Entity.hh>
#include <cereal/types/memory.hpp>
#include <Render/Key.hh>

namespace AGE
{
	struct MeshInstance;
	struct MaterialSetInstance;
	struct PrepareKey;
	struct AnimationInstance;
	class AScene;

	struct MeshRenderer : public ComponentBase
	{
		MeshRenderer();
		virtual ~MeshRenderer();
		MeshRenderer(MeshRenderer &&o);
		MeshRenderer &operator=(MeshRenderer &&o);

		void init(AScene *, std::shared_ptr<AGE::MeshInstance> file = nullptr);
		virtual void reset(AScene *);

		template <typename Archive> void save(Archive &ar) const;
		template <typename Archive> void load(Archive &ar);


		MeshRenderer &setMesh(const std::shared_ptr<AGE::MeshInstance> &_mesh);
		std::shared_ptr<AGE::MeshInstance> getMesh();
		MeshRenderer &setMaterial(const std::shared_ptr<AGE::MaterialSetInstance> &_mesh);
		std::shared_ptr<AGE::MaterialSetInstance> getMaterial();
		MeshRenderer &setAnimation(const gl::Key<AGE::AnimationInstance> &key);

		virtual void postUnserialization(AScene *scene);

	private:
		AGE::PrepareKey _key;
		AScene *_scene;
		std::shared_ptr<AGE::MeshInstance> _mesh;
		std::shared_ptr<AGE::MaterialSetInstance> _material;
		gl::Key<AGE::AnimationInstance> _animation;

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

