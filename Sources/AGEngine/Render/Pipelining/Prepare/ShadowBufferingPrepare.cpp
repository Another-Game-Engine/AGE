#include "ShadowBufferingPrepare.hpp"

// for ShadowCasterSpotLightOccluder
#include <glm/gtc/type_ptr.hpp>

// for ShadowCasterBFCCallback
#include <Graphic/DRBMeshData.hpp>
#include <Graphic/DRBMesh.hpp>

//for ShadowCasterResult
#include <BFC/BFCBlockManagerFactory.hpp>
#include <Render\Pipelining\Prepare\ShadowBufferingPrepare.hpp>
#include <Graphic/BFCCullableTypes.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <TMQ/Queue.hpp>
#include <Utils/Frustum.hh>

#include "Utils/Profiler.hpp"

namespace AGE
{
	const float ShadowCasterSpotLightOccluder::invalidVector[4] = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };


	ShadowCasterSpotLightOccluder::ShadowCasterSpotLightOccluder()
		: matrix({ std::array < float, 4 > { 1, 2, 3, 4 }, std::array < float, 4 > { 1, 2, 3, 4 }, std::array < float, 4 > { 1, 2, 3, 4 }, std::array < float, 4 > { 1, 2, 3, 4 } })
	{}

	ShadowCasterSpotLightOccluder::ShadowCasterSpotLightOccluder(glm::mat4 &&mat)
	{
		memcpy(&matrix, glm::value_ptr(mat), sizeof(glm::mat4));
	}
	ShadowCasterSpotLightOccluder::ShadowCasterSpotLightOccluder(ConcatenatedKey &&k)
	{
		keyHolder.key = std::move(k);
		keyHolder.size = 0;
		memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
	}
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	ShadowCasterResult::ShadowCasterResult(moodycamel::ConcurrentQueue<ShadowCasterBFCCallback*> *cullerPool
		, moodycamel::ConcurrentQueue<ShadowCasterResult*> *cullingResultPool)
	{
		_matrixBufferIndex = 0;
		_matrixBufferSize = 2048;
		_matrixBuffer = (ShadowCasterMatrixHandler*)malloc(sizeof(ShadowCasterMatrixHandler) * _matrixBufferSize);

		_commandBufferIndex = 0;
		_commandBufferSize = 2048;
		_commandBuffer = (ShadowCasterSpotLightOccluder*)malloc(sizeof(ShadowCasterSpotLightOccluder) * _commandBufferSize);

		_cullerPool = cullerPool;
		_cullingResultPool = cullingResultPool;
	}
	ShadowCasterResult::~ShadowCasterResult()
	{
		free(_matrixBuffer);
		free(_commandBuffer);
	}

	void ShadowCasterResult::prepareForComputation(glm::mat4 spotMat)
	{
		SCOPE_profile_cpu_function("ShadowCaster");

		_commandBufferIndex = 0;

		// matrix buffer dont't have enough place
		if (_matrixBufferSize <= _matrixBufferIndex)
		{
		}
		_matrixBufferIndex = 0;
		_taskCounter = 0;
		_spotMatrix = spotMat;

	}

	void ShadowCasterResult::cull(BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *globalCounter)
	{
		globalCounter->fetch_add(1);

		auto meshBlocksToCullNumber = bf->getBlockNumberToCull(BFCCullableType::CullableMesh);
		_globalCounter = globalCounter;
		_taskCounter = meshBlocksToCullNumber;

		for (std::size_t i = 0; i < meshBlocksToCullNumber; ++i)
		{
			ShadowCasterBFCCallback *culler = nullptr;
			if (_cullerPool->try_dequeue(culler) == false)
				culler = new ShadowCasterBFCCallback();
			culler->reset(i, this);

			TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::VoidFunction>([bf, i, frustum, culler]()
			{
				bf->cullOnBlock(BFCCullableType::CullableMesh, frustum, i, 1, culler);
			});
		}
	}

	void ShadowCasterResult::recycle(ShadowCasterBFCCallback *ptr)
	{
		_cullerPool->enqueue(ptr);
	}

	void ShadowCasterResult::mergeChunk(const BFCArray<ShadowCasterMatrixHandler> &array)
	{
		SCOPE_profile_cpu_function("ShadowCaster");

		auto aSize = array.size();
		std::size_t index = _matrixBufferIndex.fetch_add(aSize);
		if (index + aSize >= _matrixBufferSize)
		{
			_matrixBufferIndex.fetch_sub(aSize);
			return;
		}
		memcpy(_matrixBuffer + index, array.data(), aSize * sizeof(ShadowCasterMatrixHandler));
	}
	void ShadowCasterResult::sortAll()
	{
		SCOPE_profile_cpu_function("ShadowCaster");

		std::size_t index = _matrixBufferIndex.load();
		std::sort(_matrixBuffer, (_matrixBuffer + (index >= _matrixBufferSize ? _matrixBufferSize : index)), compare);
		computeCommandBuffer();
	}

	void ShadowCasterResult::computeCommandBuffer()
	{
		SCOPE_profile_cpu_function("ShadowCaster");

		if (_matrixBufferIndex != 0)
		{
			std::size_t max = _matrixBufferIndex.load() > _matrixBufferSize ? _matrixBufferSize : _matrixBufferIndex.load();
			std::size_t i = 0;

			ShadowCasterSpotLightOccluder *key = nullptr;
			ConcatenatedKey lastKey = -1;
			std::size_t keyCounter = 0;
			_commandBufferIndex = 0;

			while (i < max && _commandBufferIndex < _commandBufferSize)
			{
				auto &c = _matrixBuffer[i];
				if (c.key != lastKey)
				{
					if (key)
					{
						key->keyHolder.size = keyCounter;
					}
					keyCounter = 0;
					lastKey = c.key;
					if (_commandBufferIndex + 1 >= _commandBufferSize)
					{
						key = nullptr;
						break;
					}
					_commandBuffer[_commandBufferIndex] = ShadowCasterSpotLightOccluder(std::move(c.key));
					key = &(_commandBuffer[_commandBufferIndex++]);
				}
				if (_commandBufferIndex >= _commandBufferSize)
				{
					break;
				}
				_commandBuffer[_commandBufferIndex++] = ShadowCasterSpotLightOccluder(std::move(_matrixBuffer[i].matrix));
				++keyCounter;
				++i;
			}
			if (key)
				key->keyHolder.size = keyCounter;

			// command buffer dont't have enough place
			if (i < max - 1)
			{
			}
		}
		_globalCounter->fetch_sub(1);
		//_cullingResultPool->enqueue(this);
	}


	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	ShadowCasterBFCCallback::ShadowCasterBFCCallback()
	{
	}

	ShadowCasterBFCCallback::~ShadowCasterBFCCallback()
	{
	}

	void ShadowCasterBFCCallback::reset(std::size_t _blockId, ShadowCasterResult *_result)
	{
		_array.clear();
		matrixKeyArray.clear();
		blockNumber = 0;
		result = _result;
	}

	void ShadowCasterBFCCallback::operator()()
	{
		SCOPE_profile_cpu_function("ShadowCaster");

		for (ItemID i = 0; i < _array.size(); ++i)
		{
			DRBMeshData * mesh = ((DRBMesh*)(_array[i].getDrawable()))->getDatas().get();
			ShadowCasterMatrixHandler h;
			h.key = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.matrix = mesh->getTransformation();
			matrixKeyArray.push(h);
		}

		if (matrixKeyArray.size() > 0)
		{
			std::sort(matrixKeyArray.data(), (matrixKeyArray.data() + matrixKeyArray.size()), compare);
		}
		result->mergeChunk(matrixKeyArray);
		if (result->getTaskCounter()->fetch_sub(1) == 1)
		{
			result->sortAll();
		}
		result->recycle(this);
	}
}