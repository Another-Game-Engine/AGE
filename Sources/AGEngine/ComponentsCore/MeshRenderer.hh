#pragma once

#include <cereal/types/string.hpp>
#include <cereal/types/memory.hpp>
#include <glm/fwd.hpp>

#include <Components/Component.hh>

#include <Render/Pipelining/Render/RenderModes.hh>

#include <BFC/BFCCullableHandle.hpp>

namespace AGE
{

	struct MeshInstance; 
	struct MaterialSetInstance;
	struct AnimationInstance;
	class AScene;
	struct MaterialInstance;

	struct MeshRenderer : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_MeshRenderer");

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
		inline std::shared_ptr<AGE::MeshInstance> getMesh() { return _mesh; }
		inline std::shared_ptr<AGE::MaterialSetInstance> getMaterial() { return _material; }
		bool reload_material();

		void enableRenderMode(RenderModes mode);
		void disableRenderMode(RenderModes mode);

		void setSkinningMatrix(const glm::mat4 *skinningMatrix, std::size_t size);

		virtual void _copyFrom(const ComponentBase *destination);

#ifdef EDITOR_ENABLED
		std::vector<const char*> *meshFileList = nullptr;
		std::vector<const char*> *meshPathList = nullptr;
		std::size_t selectedMeshIndex = 0;
		std::string selectedMeshName = "";

		std::vector<const char*> *materialFileList = nullptr;
		std::vector<const char*> *materialPathList = nullptr;
		std::size_t selectedMaterialIndex = 0;
		std::string selectedMaterialName = "";

		virtual bool editorUpdate();
#endif

		virtual void postUnserialization();
		MeshRenderer &operator=(MeshRenderer const &) = delete;

	private:
		friend class cereal::access;
		std::shared_ptr<AGE::MeshInstance> _mesh;
		std::shared_ptr<AGE::MaterialSetInstance> _material;
		std::string _meshPath;
		std::string _materialPath;
		std::string _animationPath;
		BFCCullableHandleGroup _drawableHandle;

		RenderModeSet _renderMode;

		void _updateGeometry();
		void _resetDrawableHandle();
		MeshRenderer(MeshRenderer const &) = delete;
	};

	template <typename Archive>
	void MeshRenderer::save(Archive &ar, const std::uint32_t version) const
	{
		ar(_materialPath, _meshPath, _animationPath, _renderMode);
	}

	template <typename Archive>
	void MeshRenderer::load(Archive &ar, const std::uint32_t version)
	{
		ar(_materialPath, _meshPath, _animationPath);
		if (version >= 3)
		{
			ar(_renderMode);
		}
	}
}

CEREAL_CLASS_VERSION(AGE::MeshRenderer, 3)