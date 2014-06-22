#pragma once

#include <memory>

#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/File.hpp>

#include <OpenGL/GeometryManager.hh>

#include "Mesh.hpp"

namespace AGE
{
	class MeshManager : public Dependency
	{
	private:
		struct BitsetComparer {
			bool operator() (const std::bitset<MeshInfos::END> &b1, const std::bitset<MeshInfos::END> &b2) const
			{
				return b1.to_ulong() < b2.to_ulong();
			}
		};
	public:
		std::shared_ptr<MeshInstance> load(const File &filePath)
		{
			if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
				return _meshs[filePath.getFullName()];
			if (!filePath.exists())
			{
				std::cerr << "MeshManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
				assert(false);
			}

			std::ifstream ifs(filePath.getFullName(), std::ios::trunc | std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			MeshData data;
			ar(data);

			auto meshInstance = std::make_shared<MeshInstance>();

			meshInstance->subMeshs.resize(data.subMeshs.size());
			meshInstance->name = data.name;

			for (std::size_t i = 0; i < data.subMeshs.size(); ++i)
			{
				if (_pools.find(data.subMeshs[i].infos) == std::end(_pools))
				{
					createPool(data.subMeshs[i].infos);
				}
			}

			return nullptr; // temporary
		}
	private:
		std::map<std::bitset<MeshInfos::END>, std::pair<std::uint32_t, std::uint32_t>, BitsetComparer> _pools;
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;

		void createPool(const std::bitset<MeshInfos::END> &infos)
		{
			auto geometryManager = _dpyManager.lock()->getInstance<gl::GeometryManager>();
			assert(geometryManager != nullptr);

			std::size_t size = infos.count();

			auto indKey = geometryManager->addIndexPool();
			GLenum *typeComponent = new GLenum[size]; // {GL_FLOAT, GL_FLOAT, GL_FLOAT};			
			uint8_t *sizeTypeComponent = new uint8_t[size]; // { sizeof(float), sizeof(float), sizeof(float) };
			uint8_t *nbrComponent = new uint8_t[size];// {4, 4, 4};

			std::size_t ctr = 0;
			for (std::size_t i = 0; i < infos.size(); ++i)
			{
				if (!infos.test(i))
					continue;
				switch (MeshInfos(i))
				{
				case Positions:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;
				case Normals:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;
				case Tangents:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;
				case BiTangents:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;
				case Uvs:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 2;
					break;
				case Weights:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;
				case BoneIndices:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;
				case Colors:
					typeComponent[ctr] = GL_FLOAT;
					sizeTypeComponent[ctr] = sizeof(float);
					nbrComponent[ctr] = 4;
					break;

				default:
					break;
				}
				++ctr;
			}

			auto vpKey = geometryManager->addVertexPool(size, typeComponent, sizeTypeComponent, nbrComponent);
			geometryManager->attachIndexPoolToVertexPool(geometryManager->getVertexPool(0), geometryManager->getIndexPool(0));

			_pools.insert(std::make_pair(infos, std::make_pair(vpKey.getId(), indKey.getId())));

			delete[] typeComponent;
			delete[] sizeTypeComponent;
			delete[] nbrComponent;
		}
	};
}