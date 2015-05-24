#pragma once

#include <Components/Component.hh>
#include <Render/Textures/Texture3D.hh>
#include <Utils/Serialization/MatrixSerialization.hpp>
#include <Core/PrepareKey.hpp>
#include <Render/PipelineTypes.hpp>
#include <set>

namespace AGE
{

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
		const glm::mat4 &getProjection() const;
		void setPipeline(RenderType pipeline);
		RenderType getPipeline() const { return _pipeline; }
		void setTexture(std::shared_ptr<Texture3D> const &texture);
		//void addPipeline(RenderType pipeline);
		//void removePipeline(RenderType pipeline);
		//bool havePipeline(RenderType pipeline) const;

		template <typename Archive> void save(Archive &ar, const std::uint32_t version) const;
		template <typename Archive> void load(Archive &ar, const std::uint32_t version);
		virtual void postUnserialization();

#ifdef EDITOR_ENABLED
		virtual void editorCreate();
		virtual void editorDelete();
		virtual bool editorUpdate();
#endif
	private:
		std::shared_ptr<Texture3D> _texture;
		glm::mat4 _projection;
		AGE::PrepareKey _key;
		RenderType _pipeline;
	};

	template <typename Archive>
	void CameraComponent::save(Archive &ar, const std::uint32_t version) const
	{
		ar(cereal::make_nvp("projection", _projection));
	}

	template <typename Archive>
	void CameraComponent::load(Archive &ar, const std::uint32_t version)
	{
		ar(_projection);
	}
}

CEREAL_CLASS_VERSION(AGE::CameraComponent, 0)