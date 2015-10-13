#pragma once

#include "BFC/IBFCCullCallback.hpp"

#include <Utils/Containers/LFVector.hpp>

// for ShadowCasterSpotLightOccluder
#include "Utils/Key.hh"
#include <array>

// for ShadowCasterResult
#include <atomic>
#include <vector>

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
			std::size_t     offset; //8
		};
		union
		{
			std::array<std::array<float, 4>, 4> matrix; // 64
			KeyHolder keyHolder; // 24
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
	class SkinnedShadowCasterBFCCallback;
	struct DRBSkinnedMesh;

	class ShadowCasterResult
	{
	public:
		ShadowCasterResult(moodycamel::ConcurrentQueue<ShadowCasterBFCCallback*> *cullerPool, moodycamel::ConcurrentQueue<SkinnedShadowCasterBFCCallback*> *skinnedCullerPool , moodycamel::ConcurrentQueue<ShadowCasterResult*> *cullingResultPool);
		~ShadowCasterResult();
		void cull(BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *globalCounter);
		void prepareForComputation(glm::mat4 spotMat);
		void mergeChunk(const BFCArray<ShadowCasterMatrixHandler> &array);
		void mergeChunk(const BFCArray<DRBSkinnedMesh*> &array);
		void sortAll();
		inline std::atomic_size_t *getTaskCounter() { return &_taskCounter; }
		void recycle(ShadowCasterBFCCallback *ptr);
		void recycle(SkinnedShadowCasterBFCCallback *ptr);
		glm::mat4                  _spotMatrix;


		void computeCommandBuffer();

		std::atomic_size_t         _skinnedBufferIndex;
		std::vector<DRBSkinnedMesh*> _skinnedBuffer;

		std::atomic_size_t         _matrixBufferIndex;
		std::size_t                _matrixBufferSize;
		ShadowCasterMatrixHandler  *_matrixBuffer;

		static const std::size_t commandBufferSize = 2028;
		std::array<ShadowCasterSpotLightOccluder, commandBufferSize>    _commandBuffer;
		std::size_t matrixOffset;

		std::atomic_size_t         _taskCounter;
		std::atomic_size_t         *_globalCounter = nullptr;
		moodycamel::ConcurrentQueue<ShadowCasterBFCCallback*>  *_cullerPool;
		moodycamel::ConcurrentQueue<SkinnedShadowCasterBFCCallback*>  *_skinnedCullerPool;
		moodycamel::ConcurrentQueue<ShadowCasterResult*>  *_cullingResultPool;
	};

	class ShadowCasterBFCCallback : public IBFCCuller
	{
	public:
		ShadowCasterBFCCallback();
		~ShadowCasterBFCCallback();
		void reset(ShadowCasterResult *_result);

		BFCArray<ShadowCasterMatrixHandler> matrixKeyArray;
		ShadowCasterResult *result = nullptr;

		virtual void operator()();
	};

	class SkinnedShadowCasterBFCCallback : public IBFCCuller
	{
	public:
		SkinnedShadowCasterBFCCallback();
		~SkinnedShadowCasterBFCCallback();
		void reset(ShadowCasterResult *_result);

		BFCArray<DRBSkinnedMesh*> meshs;
		ShadowCasterResult *result = nullptr;

		virtual void operator()();
	};

	inline bool compare(const ShadowCasterMatrixHandler &a, const ShadowCasterMatrixHandler &b)
	{
		return (a.key < b.key);
	}
}