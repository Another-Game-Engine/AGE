#pragma once

#include <Core/AScene.hh>

#include <Systems/LifetimeSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/CameraSystem.hh>
#include <Components/MeshRenderer.hh>
#include <Components/Light.hh>

#include <Core/AssetsManager.hpp>

#include <Core/PrepareRenderThread.hpp>
#include <Core/PrepareRenderThreadCommand.hpp>
#include <Utils/ThreadQueueCommands.hpp>

#include <Context/IRenderContext.hh>
#include <Core/RenderThread.hpp>

#include <CONFIGS.hh>

//for test
#include <Utils/Containers/CommandQueue.hpp>

# define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Shaders/test_pipeline_1.fp"
# define DEFFERED_VERTEX_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
# define DEFFERED_FRAG_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.fp"


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

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<Engine> &&engine);

	virtual ~BenchmarkScene(void);
	void initRendering();
	virtual bool userStart();
	virtual bool userUpdate(double time);

private:
	AGE::RenderThread *_renderThread;
	gl::RenderManager *_renderManager;
	RenderKey key;

	std::size_t _frameCounter = 0;
	double _timeCounter = 0.0;
	double _maxTime = 15.0f;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25f;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
	Entity GLOBAL_CAMERA;
};