#include "MeshBufferingPrepare.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <Graphic/DRBMeshData.hpp>
#include <Graphic/DRBMesh.hpp>

namespace AGE
{
	const float MeshBuffering::CommandType::invalidVector[4] = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
	
	namespace MeshBuffering
	{
		void RawType::Treat(const BFCItem &item, BFCArray<RawType> &result)
		{
			DRBMeshData * mesh = ((DRBMesh*)(item.getDrawable()))->getDatas().get();
			RawType h;
			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
			h.material = ((DRBMesh*)(item.getDrawable()))->material;
			h.matrix = mesh->getTransformation();
			result.push(h);
		}

		bool RawType::Compare(const RawType &a, const RawType &b)
		{
			if (a.material == b.material)
			{
				return a.vertice < b.vertice;
			}
			return a.material < b.material;
		}

		RawType RawType::Invalid()
		{
			RawType invalid;
			invalid.material = nullptr;
			invalid.vertice = -1;
			return invalid;
		}

		bool RawType::operator!=(const RawType &o)
		{
			return (material != o.material || vertice != o.vertice);
		}

		//////////////////////////////////////////////////////////////////////////////////

		bool CommandType::isKeyHolder() const
		{
			return (memcmp(&matrix[3][0], &invalidVector, sizeof(invalidVector)) == 0);
		}

		void CommandType::setAsCommandKey(const RawType &raw)
		{
			keyHolder.material = raw.material;
			keyHolder.vertice = raw.vertice;
			memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
		}

		void CommandType::setAsCommandData(const RawType &raw)
		{
			memcpy(&matrix, glm::value_ptr(raw.matrix), sizeof(glm::mat4));
		}
	}
}

//#include "MeshBufferingPrepare.hpp"
//
//// for ShadowCasterSpotLightOccluder
//#include <glm/gtc/type_ptr.hpp>
//
//// for ShadowCasterBFCCallback
//#include <Graphic/DRBMeshData.hpp>
//#include <Graphic/DRBMesh.hpp>
//
////for ShadowCasterResult
//#include <BFC/BFCBlockManagerFactory.hpp>
//#include <Graphic/BFCCullableTypes.hpp>
//#include <Threads/Tasks/BasicTasks.hpp>
//#include <TMQ/Queue.hpp>
//#include <Utils/Frustum.hh>
//
//#include "Utils/Profiler.hpp"
//
//namespace AGE
//{
//	const float MeshCasterCommand::invalidVector[4] = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
//
//
//	MeshCasterCommand::MeshCasterCommand()
//		: matrix({ std::array < float, 4 > { 1, 2, 3, 4 }, std::array < float, 4 > { 1, 2, 3, 4 }, std::array < float, 4 > { 1, 2, 3, 4 }, std::array < float, 4 > { 1, 2, 3, 4 } })
//	{}
//
//	MeshCasterCommand::MeshCasterCommand(glm::mat4 &&mat)
//	{
//		memcpy(&matrix, glm::value_ptr(mat), sizeof(glm::mat4));
//	}
//	MeshCasterCommand::MeshCasterCommand(ConcatenatedKey &&k, MaterialInstance *&&mat)
//	{
//		keyHolder.vertice = std::move(k);
//		keyHolder.material = std::move(mat);
//		keyHolder.size = 0;
//		memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
//	}
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//
//
//	MeshCasterResult::MeshCasterResult(moodycamel::ConcurrentQueue<MeshCasterBFCCallback*> *cullerPool
//		, moodycamel::ConcurrentQueue<MeshCasterResult*> *cullingResultPool)
//	{
//		_cullerPool = cullerPool;
//		_cullingResultPool = cullingResultPool;
//	}
//
//	MeshCasterResult::~MeshCasterResult()
//	{
//	}
//
//	void MeshCasterResult::prepareForComputation(const glm::mat4 &cameraMat)
//	{
//		SCOPE_profile_cpu_function("MeshCaster");
//
//		_taskCounter = 0;
//		_cameraMatrix = cameraMat;
//		_meshBuffer.clear();
//		matrixOffset = 0;
//	}
//
//	void MeshCasterResult::cull(BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *globalCounter)
//	{
//		globalCounter->fetch_add(1);
//
//		auto meshBlocksToCullNumber = bf->getBlockNumberToCull(BFCCullableType::CullableMesh);
//
//		_globalCounter = globalCounter;
//		_taskCounter = meshBlocksToCullNumber;
//
//		for (std::size_t i = 0; i < meshBlocksToCullNumber; ++i)
//		{
//			MeshCasterBFCCallback *culler = nullptr;
//			if (_cullerPool->try_dequeue(culler) == false)
//				culler = new MeshCasterBFCCallback();
//			culler->reset(this);
//
//			TMQ::TaskManager::emplaceSharedTask<Tasks::Basic::VoidFunction>([bf, i, frustum, culler]()
//			{
//				bf->cullOnBlock(BFCCullableType::CullableMesh, frustum, i, 1, culler);
//			});
//		}
//	}
//
//	void MeshCasterResult::mergeChunk(const BFCArray<MeshCasterMatrixHandler> &array)
//	{
//		SCOPE_profile_cpu_function("MeshCaster");
//
//		_meshBuffer.pushChunk(array.data(), array.size());
//	}
//
//	void MeshCasterResult::sortAll()
//	{
//		SCOPE_profile_cpu_function("MeshCaster");
//
//		std::size_t index = _meshBuffer.size();
//		std::sort(_meshBuffer.data(), _meshBuffer.data() + index, compare);
//		computeCommandBuffer();
//	}
//
//	void MeshCasterResult::recycle(MeshCasterBFCCallback *ptr)
//	{
//		_cullerPool->enqueue(ptr);
//	}
//
//	void MeshCasterResult::computeCommandBuffer()
//	{
//		SCOPE_profile_cpu_function("MeshCaster");
//
//		if (_meshBuffer.size() != 0)
//		{
//			std::size_t max = _meshBuffer.size();
//			std::size_t i = 0;
//		
//			MeshCasterCommand *key = nullptr;
//			ConcatenatedKey lastVerticeKey = -1;
//			MaterialInstance *lastMaterialPtr = nullptr;
//
//			std::size_t keyCounter = 0;
//			std::size_t keyIndice = 0;
//
//			std::size_t matrixCounter = commandBufferSize - 1;
//		
//			while (i < max && matrixCounter > keyIndice)
//			{
//				auto &c = _meshBuffer[i];
//				if (c.vertice != lastVerticeKey && c.material != lastMaterialPtr)
//				{
//					if (key)
//					{
//						key->keyHolder.size = keyCounter;
//						key->keyHolder.offset = matrixCounter + 1;
//					}
//					keyCounter = 0;
//					lastVerticeKey = c.vertice;
//					lastMaterialPtr = c.material;
//					_commandBuffer[keyIndice] = MeshCasterCommand(std::move(c.vertice), std::move(c.material));
//					key = &(_commandBuffer[keyIndice++]);
//				}
//				_commandBuffer[matrixCounter--] = MeshCasterCommand(std::move(_meshBuffer[i].matrix));
//				++keyCounter;
//				++i;
//			}
//			matrixOffset = matrixCounter + 1;
//			if (key)
//			{
//				key->keyHolder.offset = matrixCounter + 1;
//				key->keyHolder.size = keyCounter;
//			}
//		}
//		_globalCounter->fetch_sub(1);
//	}
//
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//	//////////////////////////////////////////////////////////////////////
//
//	MeshCasterBFCCallback::MeshCasterBFCCallback()
//	{
//	}
//
//	MeshCasterBFCCallback::~MeshCasterBFCCallback()
//	{
//	}
//
//	void MeshCasterBFCCallback::reset(MeshCasterResult *_result)
//	{
//		_array.clear();
//		meshs.clear();
//		result = _result;
//	}
//
//	void MeshCasterBFCCallback::operator()()
//	{
//		SCOPE_profile_cpu_function("ShadowCaster");
//
//		for (ItemID i = 0; i < _array.size(); ++i)
//		{
//			DRBMeshData * mesh = ((DRBMesh*)(_array[i].getDrawable()))->getDatas().get();
//			MeshCasterMatrixHandler h;
//			h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
//			h.material = ((DRBMesh*)(_array[i].getDrawable()))->material;
//			h.matrix = mesh->getTransformation();
//			meshs.push(h);
//		}
//
//		if (meshs.size() > 0)
//		{
//			std::sort(meshs.data(), (meshs.data() + meshs.size()), compare);
//		}
//		result->mergeChunk(meshs);
//		if (result->getTaskCounter()->fetch_sub(1) == 1)
//		{
//			result->sortAll();
//		}
//		result->recycle(this);
//	}
//}