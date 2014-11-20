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
#include <Skinning/AnimationInstance.hpp>

# define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Shaders/test_pipeline_1.fp"
# define DEFFERED_VERTEX_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
# define DEFFERED_FRAG_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.fp"
# define DEFFERED_VERTEX_SHADER_ACCUM "../../Shaders/Deffered_shading/deffered_shading_lighting.vp"
# define DEFFERED_FRAG_SHADER_ACCUM "../../Shaders/Deffered_shading/deffered_shading_lighting.fp"
# define DEFERRED_FRAG_SHADER_MERGE "../../Shaders/Deffered_shading/deferred_shading_merge.fp"
# define DEFERRED_VERTEX_SHADER_MERGE "../../Shaders/Deffered_shading/deferred_shading_merge.vp"

struct Deferred_accum_light
{
	gl::Key<gl::Shader> shader;
	gl::Key<gl::Pipeline> pipeline;
	gl::Key<gl::RenderPass> renderPass;
	gl::Key<gl::RenderPostEffect> renderPostEffect;
	gl::Key<gl::Uniform> position_light;
	gl::Key<gl::Uniform> color_light;
	gl::Key<gl::Uniform> range_light;
	gl::Key<gl::Sampler> depth_buffer;
	gl::Key<gl::Sampler> normal_buffer;
	gl::Key<gl::Sampler> specular_buffer;
	gl::Key<gl::Uniform> ambiant_color;
};

struct Deferred_get_buffer_Key
{
	gl::Key<gl::Shader> shader;
	gl::Key<gl::Uniform> model_matrix;
	gl::Key<gl::RenderPass> renderPass;
	gl::Key<gl::Pipeline> pipeline;
	gl::Key<gl::Uniform> specular_color;
	gl::Key<gl::Uniform> specular_ratio;
	gl::Key<gl::Uniform> shininess;
};

struct Deffered_clean_buffer
{
	gl::Key<gl::EmptyRenderPass> emptyRenderPass;
	gl::Key<gl::Pipeline> pipeline;
};

struct Deffered_merge
{
	gl::Key<gl::Shader> shader;
	gl::Key<gl::Sampler> diffuse_buffer;
	gl::Key<gl::Sampler> light_buffer;
	gl::Key<gl::RenderPostEffect> renderPostEffect;
	gl::Key<gl::Pipeline> pipeline;
	gl::Key<gl::RenderOnScreen> renderOnScreen;
};

struct RenderKey
{
	Deffered_merge merge;
	Deffered_clean_buffer clean;
	Deferred_accum_light Accum;
	Deferred_get_buffer_Key getBuff;
	gl::Key<gl::UniformBlock> global_state;
};

class BenchmarkScene : public AScene	
{
public:
	BenchmarkScene(std::weak_ptr<AGE::Engine> &&engine);

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
	double _maxTime = 10000000005.0f;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25f;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
	Entity GLOBAL_CAMERA;
	Entity GLOBAL_CATWOMAN;
	Entity GLOBAL_LIGHT;
	Entity GLOBAL_SPONZA;
	Entity GLOBAL_FLOOR;
	Entity GLOBAL_HEAD;
	gl::Key<AGE::AnimationInstance> GLOBAL_CAT_ANIMATION;
};