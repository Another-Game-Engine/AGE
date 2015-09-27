#pragma once

#include <ostream>
#include <fstream>
#include <string>

#include <Core/AScene.hh>
#include <Systems/LifetimeSystem.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <ComponentsCore/MeshRenderer.hh>
#include <ComponentsCore/Light.hh>
#include <ComponentsCore/CameraComponent.hpp>
#include <AssetManagement/AssetManager.hh>
#include <Context/IRenderContext.hh>
#include <CONFIGS.hh>
#include <AssetManagement/Instance/AnimationInstance.hh>
#include <Render/Textures/TextureCubeMap.hh>

# define VERTEX_SHADER "../../Datas/Shaders/test_pipeline_1.vp"
# define FRAG_SHADER "../../Datas/Shaders/test_pipeline_1.fp"
# define DEFFERED_VERTEX_SHADER "../../Datas/Shaders/Deffered_shading/deffered_shading_get_buffer.vp"
# define DEFFERED_FRAG_SHADER "../../Datas/Shaders/Deffered_shading/deffered_shading_get_buffer.fp"
# define DEFFERED_VERTEX_SHADER_ACCUM "../../Datas/Shaders/Deffered_shading/deffered_shading_lighting.vp"
# define DEFFERED_FRAG_SHADER_ACCUM "../../Datas/Shaders/Deffered_shading/deffered_shading_lighting.fp"
# define DEFERRED_FRAG_SHADER_MERGE "../../Datas/Shaders/Deffered_shading/deferred_shading_merge.fp"
# define DEFERRED_VERTEX_SHADER_MERGE "../../Datas/Shaders/Deffered_shading/deferred_shading_merge.vp"

namespace AGE
{
	class BenchmarkScene : public AGE::AScene
	{
	public:
		BenchmarkScene(Engine *engine);

		virtual ~BenchmarkScene(void);
		void initRendering();
		virtual bool _userStart();
		virtual bool _userUpdateBegin(float time);
		virtual bool _userUpdateEnd(float time);

	private:
		float _chunkCounter = 0.0;
		float _maxChunk = 0.25f;
		std::size_t _chunkFrame = 0;
		AGE::Entity GLOBAL_CAMERA;
		int pipelineIndex = 1;
		std::shared_ptr<TextureCubeMap> _skyboxSpace;
	};
}