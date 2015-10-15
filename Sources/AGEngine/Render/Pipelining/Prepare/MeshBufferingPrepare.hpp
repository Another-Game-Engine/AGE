#pragma once

#include "BFC/BFCOutput.hpp"

namespace AGE
{
	namespace MeshBuffering
	{
		struct MeshRawType
		{
			static void Treat(const BFCItem &item, BFCArray<MeshRawType> &result)
			{
				DRBMeshData * mesh = ((DRBMesh*)(item.getDrawable()))->getDatas().get();
				MeshRawType h;
				h.vertice = ConcatenateKey(mesh->getPainterKey(), mesh->getVerticesKey());
				h.material = ((DRBMesh*)(item.getDrawable()))->material;
				h.matrix = mesh->getTransformation();
				result.push(h);
			}

			static bool Compare(const MeshRawType &a, const MeshRawType &b)
			{
				if (a.material == b.material)
				{
					return a.vertice < b.vertice;
				}
				return a.material < b.material;
			}

			static MeshRawType Invalid()
			{
				MeshRawType invalid;
				invalid.material = nullptr;
				invalid.vertice  = -1;
			}

			bool operator!=(const MeshRawType &o)
			{
				return (material != o.material || vertice != o.vertice);
			}

			ConcatenatedKey     vertice;
			MaterialInstance    *material;
			glm::mat4           matrix;    
		};

		struct MeshCommandType
		{
			static const float invalidVector[4];

			struct KeyHolder
			{
				ConcatenatedKey  vertice; // 8
				MaterialInstance *material; // 8
			};
			union
			{
				std::array<std::array<float, 4>, 4> matrix; // 64
				KeyHolder keyHolder; // 16
			};

			MeshCasterCommand();
			MeshCasterCommand(glm::mat4 &&mat);
			MeshCasterCommand(ConcatenatedKey &&k, MaterialInstance *&&material);

			inline bool isKeyHolder() const
			{
				return (memcmp(&matrix[3][0], &invalidVector, sizeof(invalidVector)) == 0);
			}

			inline void setAsCommandKey(const MeshRawType &raw)
			{
				keyHolder.material = raw.material;
				keyHolder.vertice  = raw.vertice;
				memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
			}

			inline void setAsCommandData(const MeshRawType &raw)
			{
				matrix = raw.matrix;
			}
		};

		typedef BFCCommand<MeshRawType> MeshCommand;
	}
}

//
//#include "BFC/IBFCCullCallback.hpp"
//
//#include <Utils/Containers/LFVector.hpp>
//
//// for MeshCasterCommand
//#include "Utils/Key.hh"
//#include <array>
//
//// for MeshCasterResult
//#include <atomic>
//#include <vector>
//
//#include <concurrentqueue/concurrentqueue.h>
//
//namespace AGE
//{
//	struct DRBMeshData;
//	class BFCBlockManagerFactory;
//	class Frustum;
//	struct MaterialInstance;
//
//	class MeshCasterBFCCallback;
//
//	struct MeshCasterCommand
//	{
//		static const float invalidVector[4];
//
//		struct KeyHolder
//		{
//			ConcatenatedKey vertice; // 8
//			MaterialInstance    *material; // 8
//			std::size_t     size; // 8
//			std::size_t     offset; //8
//		};
//		union
//		{
//			std::array<std::array<float, 4>, 4> matrix; // 64
//			KeyHolder keyHolder; // 32
//		};
//
//		MeshCasterCommand();
//		MeshCasterCommand(glm::mat4 &&mat);
//		MeshCasterCommand(ConcatenatedKey &&k, MaterialInstance *&&material);
//
//		inline bool isKeyHolder() const
//		{
//			return (memcmp(&matrix[3][0], &invalidVector, sizeof(invalidVector)) == 0);
//		}
//	};
//
//	struct MeshCasterMatrixHandler
//	{
//		ConcatenatedKey vertice; // painter and vertice id
//		MaterialInstance *material;
//		glm::mat4 matrix;
//		MeshCasterMatrixHandler() : vertice(-1), material(nullptr), matrix(glm::mat4(1)) {}
//		MeshCasterMatrixHandler(MeshCasterMatrixHandler &&o) : vertice(std::move(o.vertice)), material(std::move(o.material)), matrix(std::move(o.matrix)) {}
//	};
//
//	class MeshCasterResult
//	{
//	public:
//		MeshCasterResult(moodycamel::ConcurrentQueue<MeshCasterBFCCallback*> *cullerPool, moodycamel::ConcurrentQueue<MeshCasterResult*> *cullingResultPool);
//		~MeshCasterResult();
//		void cull(BFCBlockManagerFactory *bf, Frustum frustum, std::atomic_size_t *globalCounter);
//		void prepareForComputation(const glm::mat4 &cameraMat);
//		void sortAll();
//		void mergeChunk(const BFCArray<MeshCasterMatrixHandler> &array);
//		void recycle(MeshCasterBFCCallback *ptr);
//		inline std::atomic_size_t *getTaskCounter() { return &_taskCounter; }
//
//		glm::mat4                  _cameraMatrix;
//
//		void computeCommandBuffer();
//
//		static const std::size_t commandBufferSize = 16384;
//
//		LFVector<MeshCasterMatrixHandler, commandBufferSize>    _meshBuffer;
//		std::array<MeshCasterCommand, commandBufferSize>        _commandBuffer;
//		std::size_t matrixOffset;
//
//		std::atomic_size_t         _taskCounter;
//		std::atomic_size_t         *_globalCounter = nullptr;
//		moodycamel::ConcurrentQueue<MeshCasterBFCCallback*>  *_cullerPool;
//		moodycamel::ConcurrentQueue<MeshCasterResult*>       *_cullingResultPool;
//	};
//
//	class MeshCasterBFCCallback : public IBFCCuller
//	{
//	public:
//		MeshCasterBFCCallback();
//		~MeshCasterBFCCallback();
//		void reset(MeshCasterResult *_result);
//
//		BFCArray<MeshCasterMatrixHandler> meshs;
//		MeshCasterResult *result = nullptr;
//
//		virtual void operator()();
//	};
//
//	inline bool compare(const MeshCasterMatrixHandler &a, const MeshCasterMatrixHandler &b)
//	{
//		if (a.material == b.material)
//		{
//			return (a.vertice < b.vertice);
//		}
//		return (a.material < b.material);
//	}
//}