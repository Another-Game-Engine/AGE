#pragma once

#include <Core/AScene.hh>
#include <Render/Key.hh>
#include <fstream>

namespace gl 
{ 
	class Shader; 
	struct Uniform; 
	class UniformBlock; 
	class RenderPass; 
	class RenderOnScreen; 
	class Pipeline; 
}

//#include <glm/gtc/matrix_transform.hpp>

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