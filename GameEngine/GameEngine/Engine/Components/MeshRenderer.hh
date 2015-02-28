#pragma once

#include <Components/Component.hh>
#include <cereal/types/string.hpp>
#include <Entities/Entity.hh>
#include <cereal/types/memory.hpp>
#include <Render/Key.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>

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

		void init(AScene *
			, std::shared_ptr<AGE::MeshInstance> mesh = nullptr
			, std::shared_ptr<AGE::MaterialSetInstance> material = nullptr);
		virtual void reset(AScene *);

		template <typename Archive> void save(Archive &ar) const;
		template <typename Archive> void load(Archive &ar);


		bool setMeshAndMaterial(
			const std::shared_ptr<AGE::MeshInstance> &_mesh,
			const std::shared_ptr<AGE::MaterialSetInstance> &_material);
		std::shared_ptr<AGE::MeshInstance> getMesh();
		std::shared_ptr<AGE::MaterialSetInstance> getMaterial();

#ifdef EDITOR_ENABLED
		std::vector<const char*> *meshFileList = nullptr;
		std::vector<const char*> *meshPathList = nullptr;
		std::size_t selectedMeshIndex = 0;
		std::string selectedMeshName = "";
		std::string selectedMeshPath = "";

		std::vector<const char*> *materialFileList = nullptr;
		std::vector<const char*> *materialPathList = nullptr;
		std::size_t selectedMaterialIndex = 0;
		std::string selectedMaterialName = "";
		std::string selectedMaterialPath = "";

		virtual void editorCreate(AScene *scene);
		virtual void editorDelete(AScene *scene);
		virtual void editorUpdate(AScene *scene);
#endif

		virtual void postUnserialization(AScene *scene);

	private:
		AGE::PrepareKey _key;
		AScene *_scene;
		std::shared_ptr<AGE::MeshInstance> _mesh;
		std::shared_ptr<AGE::MaterialSetInstance> _material;

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

		void _updateGeometry();
		MeshRenderer(MeshRenderer const &) = delete;
		MeshRenderer &operator=(MeshRenderer const &) = delete;
	};

	template <typename Archive>
	void MeshRenderer::save(Archive &ar) const
	{
		auto serializationInfos = std::make_unique<SerializationInfos>();
#ifndef EDITOR_ENABLED
		if (_material)
		{
			serializationInfos->material = _material->path;
		}
		if (_mesh)
		{
			serializationInfos->mesh = _mesh->path;
		}
#else
		serializationInfos->material = selectedMaterialPath;
		serializationInfos->mesh = selectedMeshPath;
#endif
		ar(serializationInfos);
	}

	template <typename Archive>
	void MeshRenderer::load(Archive &ar)
	{
		ar(_serializationInfos);
	}
}