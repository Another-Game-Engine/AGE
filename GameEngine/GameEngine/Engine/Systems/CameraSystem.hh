#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Components/CameraComponent.hpp>
#include <Core/Drawable.hh>
#include <Core/RenderThread.hpp>

class CameraSystem : public System
{
public:
	CameraSystem(std::weak_ptr<AScene> &&scene);
	virtual ~CameraSystem(){}
	
	void setManager();
	void setRenderDebugMode(bool t);
	bool getRenderDebugMode() const;
	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to);
	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to);
	double getLifeTime() const;

protected:

	bool _renderDebugMethod;
	double	_totalTime;
	EntityFilter _drawable;
	EntityFilter _camera;
	AGE::RenderThread *_renderThread;
	gl::RenderManager *_renderManager;
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
//	gl::Key<gl::RenderPostEffect> _renderPostEffect;
	gl::Key<gl::RenderOnScreen> _renderOnScreen;
	gl::Key<gl::Pipeline> _pipeline;

	gl::Key<gl::Render> _renderQuad;
	gl::Key<gl::Uniform> _textureQuad;

	gl::Key<gl::Uniform> _transformation;

	AGE::Vector<AGE::DrawableCollection> _drawList;
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};