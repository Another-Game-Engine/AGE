#pragma once

#include <Components/Component.hh>
#include <cereal/types/string.hpp>
#include <Entities/Entity.hh>
#include <cereal/types/memory.hpp>
#include <Render/Key.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <Render/Pipelining/Render/RenderModes.hh>

#ifdef EDITOR_ENABLED
#include <WorldEditorGlobal.hpp>
#endif

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

		void init(std::shared_ptr<AGE::MeshInstance> mesh = nullptr
			, std::shared_ptr<AGE::MaterialSetInstance> material = nullptr);
		virtual void reset();

		template <typename Archive> void save(Archive &ar, const std::uint32_t version) const;
		template <typename Archive> void load(Archive &ar, const std::uint32_t version);


		bool setMeshAndMaterial(
			const std::shared_ptr<AGE::MeshInstance> &_mesh,
			const std::shared_ptr<AGE::MaterialSetInstance> &_material);
		std::shared_ptr<AGE::MeshInstance> getMesh();
		std::shared_ptr<AGE::MaterialSetInstance> getMaterial();

		void enableRenderMode(RenderModes mode);
		void disableRenderMode(RenderModes mode);

		virtual void _copyFrom(const ComponentBase *destination);

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

		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif

		virtual void postUnserialization();
		MeshRenderer &operator=(MeshRenderer const &) = delete;

	private:
		friend class cereal::access;
		AGE::PrepareKey _key;
		std::shared_ptr<AGE::MeshInstance> _mesh;
		std::shared_ptr<AGE::MaterialSetInstance> _material;
		RenderModeSet _renderMode;

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
	};

	template <typename Archive>
	void MeshRenderer::save(Archive &ar, const std::uint32_t version) const
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
#ifdef EDITOR_ENABLED
		if (WESerialization::SerializeForEditor() == true)
		{
			serializationInfos->material = selectedMaterialPath;
			serializationInfos->mesh = selectedMeshPath;
		}
#endif
		if (version < 1)
		{
			ar(serializationInfos);
		}
		else
		{
			ar(serializationInfos, _renderMode);
		}
	}

	template <typename Archive>
	void MeshRenderer::load(Archive &ar, const std::uint32_t version)
	{
		if (version < 1)
		{
			ar(_serializationInfos);
		}
		else
		{
			ar(_serializationInfos, _renderMode);
		}
	}
}

CEREAL_CLASS_VERSION(AGE::MeshRenderer, 1)