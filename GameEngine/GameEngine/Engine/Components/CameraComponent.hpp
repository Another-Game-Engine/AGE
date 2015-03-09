#pragma once

#include <Components/Component.hh>
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
		CameraComponent(CameraComponent const &o);
		CameraComponent	&operator=(CameraComponent const &o);

		void init(AScene *scene);
		virtual void reset(AScene *scene);

		void setProjection(const glm::mat4 &);
		const glm::mat4 &getProjection() const;
		void addPipeline(RenderType pipeline);
		void removePipeline(RenderType pipeline);
		bool havePipeline(RenderType pipeline) const;

		template <typename Archive> void save(Archive &ar) const;
		template <typename Archive> void load(Archive &ar);
		virtual void postUnserialization(AScene *scene);

#ifdef EDITOR_ENABLED
		virtual void editorCreate(AScene *scene);
		virtual void editorDelete(AScene *scene);
		virtual void editorUpdate(AScene *scene);
#endif
	private:
		AScene *_scene = nullptr;
		glm::mat4 _projection;
		AGE::PrepareKey _key;
		std::set < RenderType > _pipelines;
	};

	template <typename Archive>
	void CameraComponent::save(Archive &ar) const
	{
		ar(cereal::make_nvp("projection", _projection));
	}

	template <typename Archive>
	void CameraComponent::load(Archive &ar)
	{
		ar(_projection);
	}
}
