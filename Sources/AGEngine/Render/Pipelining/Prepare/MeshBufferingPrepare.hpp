#pragma once

#include "BFC/BFCOutput.hpp"

#include <utils/key.hh>

#include <glm/glm.hpp>

namespace AGE
{
	struct MaterialInstance;

	namespace MeshBuffering
	{
		struct RawType
		{
			static void Treat(const BFCItem &item, BFCArray<RawType> &result);
			static bool Compare(const RawType &a, const RawType &b);
			static RawType Invalid();
			bool operator!=(const RawType &o) const;

			ConcatenatedKey     vertice;
			MaterialInstance    *material;
			glm::mat4           matrix;
		};

		template <std::size_t CommandNbr>
		class CommandGenerator
		{
		public:
			struct CommandType
			{
				static const float invalidVector[4];

				struct KeyHolder
				{
					ConcatenatedKey  vertice; // 8
					MaterialInstance *material; // 8
					std::size_t size; // 8
					std::size_t offset; // 8
				};
				union
				{
					std::array<std::array<float, 4>, 4> matrix; // 64
					KeyHolder keyHolder; // 32
				};

				inline void setKeySizeAndOffset(std::size_t size, std::size_t offset)
				{
					keyHolder.size = size;
					keyHolder.offset = offset;
				}

				inline std::size_t getSize() const { return keyHolder.size; }
				inline std::size_t getOffset() const { return keyHolder.offset; }

				bool isKeyHolder() const
				{
					return (memcmp(&matrix[3][0], &invalidVector, sizeof(invalidVector)) == 0);
				}

				void setAsCommandKey(const RawType &raw)
				{
					keyHolder.material = raw.material;
					keyHolder.vertice = raw.vertice;
					memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
				}

				void setAsCommandData(const RawType &raw)
				{
					memcpy(&matrix, glm::value_ptr(raw.matrix), sizeof(glm::mat4));
				}
			};

			template <typename RawInfosType, std::size_t RawInfosNbr>
			void treatCulledResult(const LFVector<RawInfosType, RawInfosNbr> &rawInfos)
			{
				if (rawInfos.size() != 0)
				{
					std::size_t max = rawInfos.size();
					std::size_t i = 0;

					CommandType *key = nullptr;
					RawInfosType lastInfos = RawInfosType::Invalid();

					std::size_t keyCounter = 0;
					std::size_t keyIndice = 0;

					std::size_t dataCounter = _commands.size() - 1;

					while (i < max && dataCounter > keyIndice)
					{
						auto &c = rawInfos[i];
						if (c != lastInfos)
						{
							if (key)
							{
								key->setKeySizeAndOffset(keyCounter, dataCounter + 1);
							}
							keyCounter = 0;
							lastInfos = c;
							_commands[keyIndice].setAsCommandKey(c);
							key = &(_commands[keyIndice++]);
						}
						_commands[dataCounter--].setAsCommandData(rawInfos[i]);
						++keyCounter;
						++i;
					}
					_dataOffset = dataCounter + 1;
					if (key)
					{
						key->setKeySizeAndOffset(keyCounter, dataCounter + 1);
					}
				}
			}

			inline void reset()
			{
				_dataOffset = 0;
			}

			inline const std::array<CommandType, CommandNbr> &getCommands() const { return _commands; }
			inline std::size_t getDataOffset() const { return _dataOffset; }
			inline std::size_t getDataSize() const { return CommandNbr - _dataOffset; }
		private:
			std::array<CommandType, CommandNbr> _commands;
			std::size_t _dataOffset;
		};
		typedef BFCOutput<MeshBuffering::RawType, 16384, CommandGenerator<16384>> CullingOutput;
	}
	template <std::size_t T>
	const float MeshBuffering::CommandGenerator<T>::CommandType::invalidVector[4] = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
}


///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////


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