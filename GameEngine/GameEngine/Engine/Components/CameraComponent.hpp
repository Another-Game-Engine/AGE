#pragma once

#include <Components/Component.hh>
#include <Render/Textures/Texture3D.hh>
#include <Utils/Serialization/MatrixSerialization.hpp>
#include <Core/PrepareKey.hpp>
#include <Render/PipelineTypes.hpp>
#include <set>

namespace AGE
{
	struct CameraData
	{
		std::shared_ptr<Texture3D> texture;
		glm::mat4 projection = glm::mat4(1.0f);
		RenderType pipeline = RenderType::DEFERRED;
	};

	struct CameraComponent : public ComponentBase
	{
		CameraComponent();
		virtual ~CameraComponent();
		CameraComponent(CameraComponent const &o) = delete;
		CameraComponent	&operator=(CameraComponent const &o) = delete;

		virtual void _copyFrom(const ComponentBase *model);

		void init();
		virtual void reset();

		void setProjection(const glm::mat4 &);
		const glm::mat4 &getProjection() const { return _data.projection; }
		void setPipeline(RenderType pipeline);
		RenderType getPipeline() const { return _data.pipeline; }
		void setTexture(std::shared_ptr<Texture3D> const &texture);
		void addSkyBoxToChoice(std::string const &type, std::shared_ptr<Texture3D> const &texture);

		template <typename Archive> void save(Archive &ar, const std::uint32_t version) const;
		template <typename Archive> void load(Archive &ar, const std::uint32_t version);
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif
	private:
		std::unordered_map<std::string, std::shared_ptr<Texture3D>> _choicesSkymap;
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