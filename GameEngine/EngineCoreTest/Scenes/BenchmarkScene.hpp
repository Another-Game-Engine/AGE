#pragma once

#include <ostream>
#include <fstream>
#include <string>

#include <Core/AScene.hh>
#include <Systems/LifetimeSystem.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Systems/BulletDynamicSystem.hpp>
#include <Components/MeshRenderer.hh>
#include <Components/Light.hh>
#include <Components/CameraComponent.hpp>
#include <AssetManagement/AssetManager.hh>
#include <Context/IRenderContext.hh>
#include <CONFIGS.hh>
#include <AssetManagement/Instance/AnimationInstance.hh>

# define VERTEX_SHADER "../../Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Shaders/test_pipeline_1.fp"
# define DEFFERED_VERTEX_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
# define DEFFERED_FRAG_SHADER "../../Shaders/Deffered_shading/deffered_shading_get_buffer.fp"
# define DEFFERED_VERTEX_SHADER_ACCUM "../../Shaders/Deffered_shading/deffered_shading_lighting.vp"
# define DEFFERED_FRAG_SHADER_ACCUM "../../Shaders/Deffered_shading/deffered_shading_lighting.fp"
# define DEFERRED_FRAG_SHADER_MERGE "../../Shaders/Deffered_shading/deferred_shading_merge.fp"
# define DEFERRED_VERTEX_SHADER_MERGE "../../Shaders/Deffered_shading/deferred_shading_merge.vp"


class BenchmarkScene : public AGE::AScene	
{
public:
	BenchmarkScene(std::weak_ptr<AGE::Engine> engine);

	virtual ~BenchmarkScene(void);
	void initRendering();
	virtual bool userStart();
	virtual bool userUpdateBegin(double time);
	virtual bool userUpdateEnd(double time);

private:

	std::size_t _frameCounter = 0;
	double _timeCounter = 0.0;
	double _maxTime = 10000000005.0f;
	double _chunkCounter = 0.0;
	double _maxChunk = 0.25f;
	std::size_t _chunkFrame = 0;
	std::ofstream _logFile;
	AGE::Entity GLOBAL_CAMERA;
	AGE::Entity GLOBAL_CATWOMAN;
	std::array<AGE::Entity, 5> GLOBAL_LIGHTS;
	AGE::Entity GLOBAL_SPONZA;
	AGE::Entity GLOBAL_FLOOR;
	AGE::Entity GLOBAL_HEAD;
	static bool initRenderingJustOneTime;
	std::list<std::future<bool>> assetLoadingList;
};
