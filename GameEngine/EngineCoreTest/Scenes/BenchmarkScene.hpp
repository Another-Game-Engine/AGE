#pragma once

#include <Core/AScene.hh>

#include <Systems/LifetimeSystem.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include <Systems/BulletDynamicSystem.hpp>
#include <Systems/CollisionAdderSystem.hpp>
#include <Systems/CollisionCleanerSystem.hpp>
#include <Systems/CameraSystem.hh>
#include <Components\/MeshRenderer.hh>
#include <Components/PointLight.hh>

#include <Core/AssetsManager.hpp>

#include <Core/Octree.hpp>

#include <Context/IRenderContext.hh>

#include <CONFIGS.hpp>

#include <Render/RenderManager.hh>

struct RenderKey
{
	gl::Key<gl::Shader> shader;
	gl::Key<gl::UniformBlock> global_state;
	gl::Key<gl::Uniform> pro_matrix;
	gl::Key<gl::Uniform> view_matrix;
	gl::Key<gl::Uniform> normal_matrix;
	gl::Key<gl::Sampler> diffuse_texture;
	gl::Key<gl::RenderPass> renderPass;
	gl::Key<gl::RenderOnScreen> renderOnScreen;
	gl::Key<gl::Pipeline> pipeline;
};

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<Engine> &&engine);
	virtual ~BenchmarkScene(void);

	BenchmarkScene &initRenderManager();
	BenchmarkScene &initLifeTime();
	BenchmarkScene &initLogFramerate();
	BenchmarkScene &initPhysicSimulation();
	BenchmarkScene &initScene();
	virtual bool userStart();
	virtual bool userUpdate(double time);

private:
	std::size_t _frameCounter = 0;
	double _timeCounter = 0.0;
	double _maxTime = 15.0f;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25f;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
	Entity GLOBALE_CAMERA;
	RenderKey key; // rendering key
};