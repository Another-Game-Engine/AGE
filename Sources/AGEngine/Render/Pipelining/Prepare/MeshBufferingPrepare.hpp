#pragma once

#include "BFC/BFCOutput.hpp"

#include <utils/key.hh>

#include <glm/glm.hpp>

// to move in .cpp
#include <glm/gtc/type_ptr.hpp>

#include <Utils/Containers/PODVector.hpp>
#include <vector>

namespace AGE
{
	struct MaterialInstance;

	namespace BasicCommandGeneration
	{
		struct MeshRawType
		{
			static bool Treat(const BFCItem &item, BFCArray<MeshRawType> &result);
			static bool Compare(const MeshRawType &a, const MeshRawType &b);
			static MeshRawType Invalid();
			bool operator!=(const MeshRawType &o) const;

			ConcatenatedKey     vertice;
			const MaterialInstance    *material;
			glm::mat4           matrix;
		};
		struct ShadowRawType
		{
			static bool Treat(const BFCItem &item, BFCArray<ShadowRawType> &result);
			static bool Compare(const ShadowRawType &a, const ShadowRawType &b);
			static ShadowRawType Invalid();
			bool operator!=(const ShadowRawType &o) const;

			ConcatenatedKey     vertice;
			glm::mat4           matrix;
		};
		struct SkinnedMeshRawType
		{
			static bool Treat(const BFCItem &item, BFCArray<SkinnedMeshRawType> &result);
			static bool Compare(const SkinnedMeshRawType &a, const SkinnedMeshRawType &b);
			static SkinnedMeshRawType Invalid();
			bool operator!=(const SkinnedMeshRawType &o) const;

			ConcatenatedKey     vertice;
			const MaterialInstance    *material;
			glm::mat4           matrix;
			size_t              bonesIndex;
		};
		struct SkinnedShadowRawType
		{
			static bool Treat(const BFCItem &item, BFCArray<SkinnedShadowRawType> &result);
			static bool Compare(const SkinnedShadowRawType &a, const SkinnedShadowRawType &b);
			static SkinnedShadowRawType Invalid();
			bool operator!=(const SkinnedShadowRawType &o) const;

			ConcatenatedKey     vertice;
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
				const MaterialInstance *material;
			};

			void reset()
			{
				begin();
			}

			void begin()
			{
				_dataIndex = 0;
				_commandIndex = 0;
				_commands.clear();
				_datas.clear();
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
				memcpy(_datas.copy_back(), glm::value_ptr(infos.matrix), sizeof(float) * 16);
				++_currentDataIndex;
			}

			std::size_t            _currentCommandIndex;
			std::size_t            _currentDataIndex;

			std::size_t            _commandIndex;
			std::size_t            _dataIndex;
			PODVector<Command>     _commands;
			PODVector<float[16]>   _datas;
		};

		struct SkinnedMeshCommandOutput
		{

			struct Command
			{
				std::size_t from;
				std::size_t size;
				ConcatenatedKey verticeKey;
				const MaterialInstance *material;
				std::size_t bonesIndex;
			};

			void reset()
			{
				begin();
			}

			void begin()
			{
				_dataIndex = 0;
				_commandIndex = 0;
				_commands.clear();
				_datas.clear();
				_currentCommandIndex = -1;
				_currentDataIndex = 0;
			}

			~SkinnedMeshCommandOutput()
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

			void setKeyInfos(const SkinnedMeshRawType &infos)
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
				command->bonesIndex = infos.bonesIndex;
			}

			void setCommandData(const SkinnedMeshRawType &infos)
			{
				memcpy(_datas.copy_back(), glm::value_ptr(infos.matrix), sizeof(float) * 16);
				++_currentDataIndex;
			}

			std::size_t            _currentCommandIndex;
			std::size_t            _currentDataIndex;

			std::size_t            _commandIndex;
			std::size_t            _dataIndex;
			PODVector<Command>     _commands;
			PODVector<float[16]>   _datas;
		};

		struct SkinnedShadowCommandOutput
		{

			struct Command
			{
				std::size_t from;
				std::size_t size;
				ConcatenatedKey verticeKey;
				std::size_t bonesIndex;
			};

			void reset()
			{
				begin();
			}

			void begin()
			{
				_dataIndex = 0;
				_commandIndex = 0;
				_commands.clear();
				_datas.clear();
				_currentCommandIndex = -1;
			}

			~SkinnedShadowCommandOutput()
			{
			}

			SkinnedShadowCommandOutput()
			{
			}

			void end()
			{
				if (_currentCommandIndex != -1)
				{
					Command *command = nullptr;
					command = &_commands[_currentCommandIndex];
					command->size = _datas.size() - command->from;
				}
			}

			void setKeyInfos(const SkinnedShadowRawType &infos)
			{
				Command *command = nullptr;
				if (_currentCommandIndex != -1)
				{
					command = &_commands[_currentCommandIndex];
					command->size = _datas.size() - command->from;
				}
				_commands.push_back(Command());
				command = &_commands.back();
				_currentCommandIndex = _commands.size() - 1;

				command->from = _datas.size();
				command->verticeKey = infos.vertice;
				command->bonesIndex = infos.bonesIndex;
			}

			void setCommandData(const SkinnedShadowRawType &infos)
			{
				memcpy(_datas.copy_back(), glm::value_ptr(infos.matrix), sizeof(float) * 16);
			}

			std::size_t            _currentCommandIndex;

			std::size_t            _commandIndex;
			std::size_t            _dataIndex;
			PODVector<Command>     _commands;
			PODVector<float[16]>   _datas;

			glm::mat4              _spotLightMatrix;
		};

		struct MeshShadowCommandOutput
		{

			struct Command
			{
				std::size_t from;
				std::size_t size;
				ConcatenatedKey verticeKey;
			};

			void reset()
			{
				begin();
			}

			void begin()
			{
				_dataIndex = 0;
				_commandIndex = 0;
				_commands.clear();
				_datas.clear();
				_currentCommandIndex = -1;
			}

			~MeshShadowCommandOutput()
			{
			}

			MeshShadowCommandOutput()
			{
			}

			void end()
			{
				if (_currentCommandIndex != -1)
				{
					Command *command = nullptr;
					command = &_commands[_currentCommandIndex];
					command->size = _datas.size() - command->from;
				}
			}

			void setKeyInfos(const ShadowRawType &infos)
			{
				Command *command = nullptr;
				if (_currentCommandIndex != -1)
				{
					command = &_commands[_currentCommandIndex];
					command->size = _datas.size() - command->from;
				}
				_commands.push_back(Command());
				command = &_commands.back();
				_currentCommandIndex = _commands.size() - 1;

				command->from = _datas.size();
				command->verticeKey = infos.vertice;
			}

			void setCommandData(const ShadowRawType &infos)
			{
				memcpy(_datas.copy_back(), glm::value_ptr(infos.matrix), sizeof(float) * 16);
			}

			std::size_t            _currentCommandIndex;

			std::size_t            _commandIndex;
			std::size_t            _dataIndex;
			PODVector<Command>     _commands;
			PODVector<float[16]>   _datas;

			glm::mat4              _spotLightMatrix;
		};

		typedef BFCOutput<BasicCommandGeneration::MeshRawType, 16384, MeshCommandOutput> MeshAndMaterialOutput;
		typedef BFCOutput<BasicCommandGeneration::ShadowRawType, 16384, MeshShadowCommandOutput> MeshShadowOutput;
		typedef BFCOutput<BasicCommandGeneration::SkinnedShadowRawType, 4096, SkinnedShadowCommandOutput> SkinnedShadowOutput;
		typedef BFCOutput<BasicCommandGeneration::SkinnedMeshRawType, 4096, SkinnedMeshCommandOutput> SkinnedMeshAndMaterialOutput;
	}
}

