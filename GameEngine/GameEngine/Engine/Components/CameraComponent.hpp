#pragma once

#include <glm/glm.hpp>
#include <Components/Component.hh>
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/string.hpp>
#include <Core/OctreeElement.hh>

namespace Component
{
	struct CameraComponent : public ComponentBase<CameraComponent>, public AGE::PrepareElement
	{
		CameraComponent();
		virtual ~CameraComponent();
		CameraComponent(CameraComponent const &o);
		CameraComponent	&operator=(CameraComponent const &o);
		void init(AScene *scene);
		virtual void reset(AScene *scene);

		void dettachSkybox();
		const std::string &getSkyboxShader() const;

		template <typename Archive> void save(Archive &ar) const;
		template <typename Archive> void load(Archive &ar);

		virtual PrepareElement &initOctree(::AScene *scene, ENTITY_ID entityId);
		virtual PrepareElement &resetOctree(::AScene *scene, ENTITY_ID entityId);

		void setProjection(const glm::mat4 &);
		const glm::mat4 &getProjection() const;

		glm::uvec4	viewport;
		std::string cubeMapShader;
		bool blitOnScreen;

		// Camera fbo infos
		glm::uvec2	fboSize;
		uint32_t	sampleNbr;
	private:
		glm::mat4 _projection;
		AScene *_scene = nullptr;

	};

	template <typename Archive>
	void CameraComponent::save(Archive &ar) const
	{
	}

	template <typename Archive>
	void CameraComponent::load(Archive &ar)
	{
	}
}
