#pragma once

#include <Utils/Debug.hpp>
#include <Utils/OpenGL.hh>
#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>
#include <SpacePartitioning/Ouptut/RenderLight.hh>
#include <glm/glm.hpp>
#include <Render/Textures/Texture2D.hh>

#include <memory>

namespace AGE
{

	class DeferredMerging;

	class DebugDeferredShading : public ARenderingPipeline
	{
	public:
		DebugDeferredShading(glm::uvec2 const &screen_size, std::shared_ptr<PaintingManager> const &painter_manager);
		DebugDeferredShading(DebugDeferredShading &&move);
		virtual ~DebugDeferredShading() {};
		DebugDeferredShading(DebugDeferredShading const &) = delete;
		DebugDeferredShading &operator=(DebugDeferredShading const &) = delete;

	public:
		void setAmbient(glm::vec3 const &ambient);

	private:
		std::shared_ptr<Texture2D> _depthStencil;
		std::shared_ptr<Texture2D> _diffuse;
		std::shared_ptr<Texture2D> _debugLights;
		std::shared_ptr<Texture2D> _normal;
		std::shared_ptr<Texture2D> _specular;
		std::shared_ptr<Texture2D> _lightAccumulation;
		std::shared_ptr<DeferredMerging> _deferredMerging;
	};

}