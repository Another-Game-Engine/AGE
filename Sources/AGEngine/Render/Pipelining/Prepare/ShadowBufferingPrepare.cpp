#include "ShadowBufferingPrepare.hpp"

// for ShadowCasterSpotLightOccluder
#include <glm/gtc/type_ptr.hpp>

// for ShadowCasterBFCCallback
#include <Graphic/DRBMeshData.hpp>
#include <Graphic/DRBMesh.hpp>

#include "Utils/Profiler.hpp"

namespace AGE
{
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

	ShadowCasterResult::ShadowCasterResult()
	{
		_matrixBufferIndex = 0;
		_matrixBufferSize = 512;
		_matrixBuffer = (ShadowCasterMatrixHandler*)malloc(sizeof(ShadowCasterMatrixHandler) * _matrixBufferSize);

		_commandBufferIndex = 0;
		_commandBufferSize = 512;
		_commandBuffer = (ShadowCasterSpotLightOccluder*)malloc(sizeof(ShadowCasterSpotLightOccluder) * _commandBufferSize);

		_commandBufferHaveEnoughPlace = true;
	}
	ShadowCasterResult::~ShadowCasterResult()
	{
		free(_matrixBuffer);
		free(_commandBuffer);
	}

	void ShadowCasterResult::prepareForComputation(std::shared_ptr<DRBSpotLightData> &spotlightData)
	{
		// command buffer dont't have enough place
		if (_commandBufferHaveEnoughPlace == false)
		{
			_commandBufferSize = _commandBufferSize * 15 / 10;
			_commandBuffer = (ShadowCasterSpotLightOccluder*)realloc(_commandBuffer, _commandBufferSize * sizeof(ShadowCasterSpotLightOccluder));
			AGE_ASSERT(_commandBuffer != nullptr);
		}
		_commandBufferIndex = 0;

		// matrix buffer dont't have enough place
		if (_matrixBufferSize >= _matrixBufferIndex)
		{
			_matrixBufferSize = _matrixBufferIndex;
			_matrixBuffer = (ShadowCasterMatrixHandler*)realloc(_matrixBuffer, _matrixBufferSize * sizeof(ShadowCasterMatrixHandler));
			AGE_ASSERT(_matrixBuffer != nullptr);
		}
		_matrixBufferIndex = 0;


	}

	void ShadowCasterResult::mergeChunk(const BFCArray<ShadowCasterMatrixHandler> &array)
	{
		auto aSize = array.size();
		std::size_t index = _matrixBufferIndex.fetch_add(aSize);
		if (index + aSize >= _matrixBufferSize)
		{
			return;
		}
		memcpy(_matrixBuffer + index, array.data(), aSize * sizeof(ShadowCasterMatrixHandler));
	}
	void ShadowCasterResult::sortAll()
	{
		std::size_t index = _matrixBufferIndex.load();
		std::sort(_matrixBuffer, (_matrixBuffer + (index >= _matrixBufferSize ? _matrixBufferSize : index)), compare);
	}

	void ShadowCasterResult::computeCommandBuffer()
	{
		if (_matrixBufferIndex == 0)
		{
			return;
		}

		std::size_t max = _matrixBufferIndex;
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
			_commandBufferHaveEnoughPlace = false;
		}
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

	void ShadowCasterBFCCallback::reset(std::size_t _blockId, std::size_t _totalBlockNumber, std::atomic_size_t *_counter)
	{
		doneCounter = _counter;
		numberOfTask = _totalBlockNumber;
		_array.clear();
		matrixKeyArray.clear();
		blockNumber = 0;
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

		doneCounter->fetch_add(1);
		if (doneCounter->load() == numberOfTask)
		{
			//TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::VoidFunction>([](){
			// TODO
			//});
		}
		
	}
}