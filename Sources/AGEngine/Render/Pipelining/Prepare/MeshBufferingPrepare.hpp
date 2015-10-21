#pragma once

#include "BFC/BFCOutput.hpp"

#include <utils/key.hh>

#include <glm/glm.hpp>

#include <vector>

namespace AGE
{
	struct MaterialInstance;

	namespace BasicCommandGeneration
	{
		struct MeshRawType
		{
			static void Treat(const BFCItem &item, BFCArray<MeshRawType> &result);
			static bool Compare(const MeshRawType &a, const MeshRawType &b);
			static MeshRawType Invalid();
			bool operator!=(const MeshRawType &o) const;

			ConcatenatedKey     vertice;
			MaterialInstance    *material;
			glm::mat4           matrix;
		};
		struct ShadowRawType
		{
			static void Treat(const BFCItem &item, BFCArray<ShadowRawType> &result);
			static bool Compare(const ShadowRawType &a, const ShadowRawType &b);
			static ShadowRawType Invalid();
			bool operator!=(const ShadowRawType &o) const;

			ConcatenatedKey     vertice;
			glm::mat4           matrix;
		};
		struct SkinnedMeshRawType
		{
			static void Treat(const BFCItem &item, BFCArray<SkinnedMeshRawType> &result);
			static bool Compare(const SkinnedMeshRawType &a, const SkinnedMeshRawType &b);
			static SkinnedMeshRawType Invalid();
			bool operator!=(const SkinnedMeshRawType &o) const;

			ConcatenatedKey     vertice;
			MaterialInstance    *material;
			glm::mat4           matrix;
			size_t              bonesIndex;
		};

		struct MeshCommandOutput
		{

			struct Command
			{
				std::size_t from;
				std::size_t size;
				ConcatenatedKey verticeKey;
				MaterialInstance *material;
			};

			void begin()
			{
				_dataIndex = 0;
				_commandIndex = 0;
				_commands.clear();
				_commands.reserve(124);
				_datas.clear();
				_datas.reserve(12000);
				_currentCommandIndex = -1;
				_currentDataIndex = 0;
			}

			~MeshCommandOutput()
			{
			}

			void end()
			{
				if (_currentCommandIndex != -1)
				{
					Command *command = nullptr;
					command = &_commands[_currentCommandIndex];
					command->size = _currentDataIndex - command->from;
				}
			}

			void setKeyInfos(const MeshRawType &infos)
			{
				Command *command = nullptr;
				if (_currentCommandIndex != -1)
				{
					command = &_commands[_currentCommandIndex];
					command->size = _currentDataIndex - command->from;
				}
				_commands.push_back(Command());
				command = &_commands.back();
				_currentCommandIndex = _commands.size() - 1;

				command->from = _currentDataIndex;
				command->verticeKey = infos.vertice;
				command->material = infos.material;
			}

			void setCommandData(const MeshRawType &infos)
			{
				_datas.push_back(infos.matrix);
				++_currentDataIndex;
			}

			std::size_t            _currentCommandIndex;
			std::size_t            _currentDataIndex;

			std::size_t            _commandIndex;
			std::size_t            _dataIndex;
			std::vector<Command>   _commands;
			std::vector<glm::mat4> _datas;
		};

		struct MeshShadowCommandOutput
		{

			struct Command
			{
				std::size_t from;
				std::size_t size;
				ConcatenatedKey verticeKey;
			};

			void begin()
			{
				_dataIndex = 0;
				_commandIndex = 0;
				_commands.reserve(124);
				_datas.clear();
				_datas.reserve(12000);
				_currentCommandIndex = -1;
				_currentDataIndex = -1;
			}

			~MeshShadowCommandOutput()
			{
			}

			void end()
			{
				if (_currentCommandIndex != -1)
				{
					Command *command = nullptr;
					command = &_commands[_currentCommandIndex];
					command->size = _currentDataIndex - command->from;
				}
			}

			void setKeyInfos(const MeshRawType &infos)
			{
				Command *command = nullptr;
				if (_currentCommandIndex != -1)
				{
					command = &_commands[_currentCommandIndex];
					command->size = _currentDataIndex - command->from;
				}
				_commands.push_back(Command());
				command = &_commands.back();
				_currentCommandIndex = _commands.size() - 1;

				command->from = _currentCommandIndex;
				command->verticeKey = infos.vertice;
			}

			void setCommandData(const MeshRawType &infos)
			{
				_datas.push_back(infos.matrix);
				++_currentDataIndex;
			}

			std::size_t            _currentCommandIndex;
			std::size_t            _currentDataIndex;

			std::size_t            _commandIndex;
			std::size_t            _dataIndex;
			std::vector<Command>   _commands;
			std::vector<glm::mat4> _datas;
		};

		//class CommandGenerator
		//{
		//public:
		//	struct CommandType
		//	{
		//		static const float invalidVector[4];

		//		struct KeyHolder
		//		{
		//			ConcatenatedKey  vertice; // 8
		//			MaterialInstance *material; // 8
		//			std::size_t size; // 8
		//			std::size_t offset; // 8
		//		};
		//		union
		//		{
		//			std::array<std::array<float, 4>, 4> matrix; // 64
		//			KeyHolder keyHolder; // 32
		//		};

		//		inline void setKeySizeAndOffset(std::size_t size, std::size_t offset)
		//		{
		//			keyHolder.size = size;
		//			keyHolder.offset = offset;
		//		}

		//		inline std::size_t getSize() const { return keyHolder.size; }
		//		inline std::size_t getOffset() const { return keyHolder.offset; }

		//		bool isKeyHolder() const
		//		{
		//			return (memcmp(&matrix[3][0], &invalidVector, sizeof(invalidVector)) == 0);
		//		}

		//		void setAsCommandKey(const MeshRawType &raw)
		//		{
		//			keyHolder.material = raw.material;
		//			keyHolder.vertice = raw.vertice;
		//			memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
		//		}

		//		void setAsCommandData(const MeshRawType &raw)
		//		{
		//			memcpy(&matrix, glm::value_ptr(raw.matrix), sizeof(glm::mat4));
		//		}

		//		void setAsCommandKey(const ShadowRawType &raw)
		//		{
		//			keyHolder.vertice = raw.vertice;
		//			memcpy(&(matrix[3][0]), &(invalidVector), sizeof(invalidVector));
		//		}

		//		void setAsCommandData(const ShadowRawType &raw)
		//		{
		//			memcpy(&matrix, glm::value_ptr(raw.matrix), sizeof(glm::mat4));
		//		}
		//	};

		//	//inline const std::array<CommandType, CommandNbr> &getCommands() const { return /_commands; }
		//	//inline std::size_t getDataOffset() const { return _dataOffset; }
		//	//inline std::size_t getDataSize() const { return CommandNbr - _dataOffset; }
		//private:
		//	std::size_t _dataOffset;
		//};
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////////


		typedef BFCOutput<BasicCommandGeneration::MeshRawType, 16384, MeshCommandOutput> MeshAndMaterialOutput;
		typedef BFCOutput<BasicCommandGeneration::ShadowRawType, 1024, MeshShadowCommandOutput> MeshShadowOutput;
	}
	//template <std::size_t T>
	//const float BasicCommandGeneration::CommandGenerator<T>::CommandType::invalidVector[4] = { std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest(), std::numeric_limits<float>::lowest() };
}

