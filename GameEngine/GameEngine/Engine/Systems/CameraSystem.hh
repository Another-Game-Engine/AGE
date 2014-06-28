#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <glm/glm.hpp>
#include <OpenGL/Key.hh>

namespace gl { class Shader; class ShadingManager; struct Uniform; class UniformBlock; class GeometryManager; }

class CameraSystem : public System
{
public:
	CameraSystem(std::weak_ptr<AScene> &&scene);
	virtual ~CameraSystem(){}

	void setManager(gl::ShadingManager &m, gl::GeometryManager &g);
	void setRenderDebugMode(bool t);
	bool getRenderDebugMode() const;
	void getRayFromMousePosOnScreen(glm::vec3 &from, glm::vec3 &to);
	void getRayFromCenterOfScreen(glm::vec3 &from, glm::vec3 &to);
	double getLifeTime() const;

protected:
	gl::ShadingManager *_render;
	gl::GeometryManager *_geometry;
	bool _renderDebugMethod;
	double	_totalTime;
	EntityFilter _drawable;
	EntityFilter _camera;
	gl::Key<gl::Shader> _shader;
	gl::Key<gl::UniformBlock> _global_state;
	gl::Key<gl::Uniform> _model_matrix;
	gl::Key<gl::Uniform> _view_matrix;
	gl::Key<gl::Uniform> _projection_matrix;

	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};