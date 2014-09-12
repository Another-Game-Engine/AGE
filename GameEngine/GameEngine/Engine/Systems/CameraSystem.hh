#pragma once

#include <Systems/System.h>
#include <Core/EntityFilter.hpp>
#include <glm/glm.hpp>
#include <Render/Key.hh>
#include <Components/CameraComponent.hpp>
#include <Core/Drawable.hh>
#include <Core/RenderThread.hpp>

namespace gl
{
	class Shader;
	struct Uniform;
	class UniformBlock;
	class RenderPass;
	class RenderOnScreen;
	class Pipeline;
}

struct RenderKey
{
	gl::Key<gl::Shader> shader;
	gl::Key<gl::UniformBlock> global_state;
	gl::Key<gl::Uniform> pro_matrix;
	gl::Key<gl::Uniform> view_matrix;
	gl::Key<gl::Uniform> normal_matrix;
	gl::Key<gl::RenderPass> renderPass;
	gl::Key<gl::RenderOnScreen> renderOnScreen;
	gl::Key<gl::Pipeline> pipeline;
};

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
	
	RenderKey key;

	AGE::Vector<AGE::DrawableCollection> _drawList;
	virtual void updateBegin(double time);
	virtual void updateEnd(double time);
	virtual void mainUpdate(double time);
	virtual bool initialize();
};