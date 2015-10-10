#pragma once

#include <Render/Pipelining/Render/FrameBufferRender.hh>
#include <list>
#include <glm/glm.hpp>

#include <concurrentqueue/concurrentqueue.h>

namespace AGE
{
	class Texture2D;
	class TextureBuffer;
	struct DRBSpotLightData;
	class ShadowCasterResult;
	class ShadowCasterBFCCallback;
	class SkinnedShadowCasterBFCCallback;
	class Frustum;
	class BFCBlockManagerFactory;

	class DeferredShadowBuffering : public FrameBufferRender
	{
	public:
		// hack for test
		static DeferredShadowBuffering *instance;

		DeferredShadowBuffering(glm::uvec2 const &screenSize, std::shared_ptr<PaintingManager> painterManager);
		virtual ~DeferredShadowBuffering() = default;
		virtual void init();

		void prepareRender(glm::mat4, BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *counter);
	protected:
		virtual void renderPass(const DRBCameraDrawableList &infos);

	private:
		std::shared_ptr<AGE::TextureBuffer> _positionBuffer = nullptr;
		static const std::size_t _maxMatrixInstancied = 1024;
		static const std::size_t _sizeofMatrix = sizeof(glm::mat4);
		static const std::size_t _maxInstanciedShadowCaster = _maxMatrixInstancied;

		moodycamel::ConcurrentQueue<ShadowCasterResult*>       _toDraw;
		moodycamel::ConcurrentQueue<ShadowCasterResult*>       _cullingResultsPool;
		moodycamel::ConcurrentQueue<ShadowCasterBFCCallback*>  _cullerPool;
		moodycamel::ConcurrentQueue<SkinnedShadowCasterBFCCallback*>  _skinnedCullerPool;
	};
}
