#pragma once

#include <Render/Pipelining/Pipelines/ARenderingPipeline.hh>

/// dirty dirty for test
#include <glm/glm.hpp>
#include <mutex>
#include <vector>

struct DirtyBoneContainer
{
	static std::vector<glm::mat4> BONES;
	static std::mutex BONES_MUTEX;

	static void setBones(const std::vector<glm::mat4> &bones)
	{
		std::lock_guard<std::mutex> lock(BONES_MUTEX);
		BONES = bones;
	}

	static std::vector<glm::mat4> getBones()
	{
		std::lock_guard<std::mutex> lock(BONES_MUTEX);
		return BONES;
	}
private:
	DirtyBoneContainer();
};



namespace AGE
{
	class BasicPipeline : public ARenderingPipeline
	{
	public:
		enum Steps
		{
			RENDER_SKINNED = 0,
			RENDER_BASIC,
			TOTAL
		};

	public:
		BasicPipeline(std::shared_ptr<PaintingManager> const &painter_manager);
		BasicPipeline(BasicPipeline &&move);
		virtual ~BasicPipeline() = default;

	public:
		virtual IRenderingPipeline &render(RenderPipeline const &pipeline, RenderLightList const &lights, CameraInfos const &camera) override final;

	};
}