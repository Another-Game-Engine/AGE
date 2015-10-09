#pragma once

#include "BFC/IBFCCullCallback.hpp"

// for ShadowCasterSpotLightOccluder
#include "Utils/Key.hh"
#include <array>

// for ShadowCasterResult
#include <atomic>

#include <concurrentqueue/concurrentqueue.h>

namespace AGE
{
	struct DRBSpotLightData;
	class BFCBlockManagerFactory;
	class Frustum;

	struct ShadowCasterSpotLightOccluder
	{
		static const float invalidVector[4];

		struct KeyHolder
		{
			ConcatenatedKey key; // 8
			std::size_t     size; // 8
		};
		union
		{
			std::array<std::array<float, 4>, 4> matrix; // 64
			KeyHolder keyHolder; // 16
		};

		ShadowCasterSpotLightOccluder();
		ShadowCasterSpotLightOccluder(glm::mat4 &&mat);
		ShadowCasterSpotLightOccluder(ConcatenatedKey &&k);

		inline bool isKeyHolder() const
		{
			return (memcmp(&matrix[3][0], &invalidVector, sizeof(invalidVector)) == 0);
		}
	};

	struct ShadowCasterMatrixHandler
	{
		ConcatenatedKey key; // painter and vertice id
		glm::mat4 matrix;
		ShadowCasterMatrixHandler() : key(-1), matrix(glm::mat4(1)) {}
		ShadowCasterMatrixHandler(ShadowCasterMatrixHandler &&o) : key(std::move(o.key)), matrix(std::move(o.matrix)) {}
	};

	class ShadowCasterBFCCallback;

	class ShadowCasterResult
	{
	public:
		ShadowCasterResult(moodycamel::ConcurrentQueue<ShadowCasterBFCCallback*> *cullerPool, moodycamel::ConcurrentQueue<ShadowCasterResult*> *cullingResultPool);
		~ShadowCasterResult();
		void cull(BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *globalCounter);
		void prepareForComputation(std::shared_ptr<DRBSpotLightData> &spotlightData);
		void mergeChunk(const BFCArray<ShadowCasterMatrixHandler> &array);
		void sortAll();
		inline std::atomic_size_t *getTaskCounter() { return &_taskCounter; }
		void recycle(ShadowCasterBFCCallback *ptr);
	private:

		void computeCommandBuffer();

		std::atomic_size_t         _matrixBufferIndex;
		std::size_t                _matrixBufferSize;
		ShadowCasterMatrixHandler  *_matrixBuffer;

		std::size_t                _commandBufferIndex;
		std::size_t                _commandBufferSize;
		ShadowCasterSpotLightOccluder       *_commandBuffer;
		bool                       _commandBufferHaveEnoughPlace;

		std::atomic_size_t         _taskCounter;
		std::atomic_size_t         *_globalCounter = nullptr;
		moodycamel::ConcurrentQueue<ShadowCasterBFCCallback*>  *_cullerPool;
		moodycamel::ConcurrentQueue<ShadowCasterResult*>  *_cullingResultPool;
	};

	class ShadowCasterBFCCallback : public IBFCCuller
	{
	public:
		ShadowCasterBFCCallback();
		~ShadowCasterBFCCallback();
		void reset(std::size_t _blockId, ShadowCasterResult *_result);

		BFCArray<ShadowCasterMatrixHandler> matrixKeyArray;
		std::size_t        blockNumber = 0;
		ShadowCasterResult *result = nullptr;

		virtual void operator()();
	};

	inline bool compare(const ShadowCasterMatrixHandler &a, const ShadowCasterMatrixHandler &b)
	{
		return (a.key < b.key);
	}
}