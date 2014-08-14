#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Components/CameraComponent.hpp>

# define NEW_SHADER 1

#if NEW_SHADER
namespace gl { class Shader; class ShadingManager; struct Uniform; class UniformBlock; class GeometryManager; struct Sampler; class RenderPass; class Render; }
#endif

class CameraSystem : public System
{
public:
	CameraSystem(std::weak_ptr<AScene> &&scene);
	virtual ~CameraSystem(){}

#if NEW_SHADER
	void setManager(gl::ShadingManager &m);
#endif
	void setRenderDebugMode(bool t);
	bool getRenderDebugMode() const;
	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to);
	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to);
	double getLifeTime() const;

protected:

	bool _renderDebugMethod;
	double	_totalTime;
#if !NEW_SHADER
	EntityFilter _filter;
#else
	EntityFilter _drawable;
	EntityFilter _camera;
	gl::ShadingManager *_render;
	gl::Key<gl::Shader> _shader;
	gl::Key<gl::Shader> _quadShader;
	gl::Key<gl::UniformBlock> _global_state;
	gl::Key<gl::Uniform> _pro_matrix;
	gl::Key<gl::Uniform> _model_matrix;
	gl::Key<gl::Uniform> _view_matrix;
	gl::Key<gl::Uniform> _normal_matrix;
	gl::Key<gl::Uniform> _diffuse_color;
	gl::Key<gl::Uniform> _diffuse_ratio;
	gl::Key<gl::Sampler> _diffuse_texture;
	gl::Key<gl::RenderPass> _renderPass;
	
	gl::Key<gl::Render> _renderQuad;
	gl::Key<gl::Uniform> _textureQuad;

	gl::Key<gl::Uniform> _transformation;
#endif
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};