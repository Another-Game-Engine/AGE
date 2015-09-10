#pragma once

#include <Components/Component.hh>
#include <Render/Textures/TextureCubeMap.hh>
#include <Utils/Serialization/MatrixSerialization.hpp>
#include <Core/PrepareKey.hpp>
#include <Render/PipelineTypes.hpp>
#include <set>

#include "Graphic/DRBCameraDrawableList.hpp"

namespace AGE
{
	struct CameraComponent : public ComponentBase
	{
		AGE_COMPONENT_UNIQUE_IDENTIFIER("AGE_CORE_CameraComponent");
	public:
		CameraComponent();
		virtual ~CameraComponent();
		CameraComponent(CameraComponent const &o) = delete;
		CameraComponent	&operator=(CameraComponent const &o) = delete;

		virtual void _copyFrom(const ComponentBase *model);

		void init();
		virtual void reset();

		void setProjection(const glm::mat4 &);
		inline const glm::mat4 &getProjection() const { return _data.projection; }
		void setPipeline(RenderType pipeline);
		RenderType getPipeline() const;
		inline const CameraData &getData() const { return _data; }
		void setTexture(std::shared_ptr<TextureCubeMap> const &texture);

		template <typename Archive> void save(Archive &ar, const std::uint32_t version) const;
		template <typename Archive> void load(Archive &ar, const std::uint32_t version);
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		void addSkyBoxToChoice(std::string const &type, std::shared_ptr<TextureCubeMap> const &texture);
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
		std::unordered_map<std::string, std::shared_ptr<TextureCubeMap>> _choicesSkymap;
#endif
	private:
		CameraData _data;
		AGE::PrepareKey _key;
	};

	template <typename Archive>
	void CameraComponent::save(Archive &ar, const std::uint32_t version) const
	{
		ar(cereal::make_nvp("projection", _data.projection));
	}

	template <typename Archive>
	void CameraComponent::load(Archive &ar, const std::uint32_t version)
	{
		ar(_data.projection);
	}
}

CEREAL_CLASS_VERSION(AGE::CameraComponent, 0)