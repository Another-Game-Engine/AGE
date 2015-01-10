#include <Core/AssetsManager.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/Material.hpp>
#include <Texture/Texture.hpp>
//#include <Render/RenderManager.hh>
#include <Threads/ThreadManager.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Threads/QueueOwner.hpp>
#include <Threads/Thread.hpp>

namespace AGE
{
	std::shared_ptr<MeshInstance> AssetsManager::getMesh(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<Animation> AssetsManager::loadAnimation(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		auto animation = std::make_shared<Animation>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(*animation.get());
		_animations.insert(std::make_pair(filePath.getFullName(), animation));
		return animation;
	}

	std::shared_ptr<Animation> AssetsManager::getAnimation(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<Skeleton> AssetsManager::getSkeleton(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<Skeleton> AssetsManager::loadSkeleton(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		auto skeleton = std::make_shared<Skeleton>();

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		ar(*skeleton.get());
		_skeletons.insert(std::make_pair(filePath.getFullName(), skeleton));
		return skeleton;
	}

	std::shared_ptr<MeshInstance> AssetsManager::loadMesh(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		MeshData data;
		ar(data);

//		auto res = AGE::GetRenderThread()->getQueue()->

		auto meshInstance = std::make_shared<MeshInstance>();

		meshInstance->subMeshs.resize(data.subMeshs.size());
		meshInstance->name = data.name;
		meshInstance->path = _filePath.getFullName();

		for (std::size_t i = 0; i < data.subMeshs.size(); ++i)
		{
			// If no vertex pool correspond to submesh
			std::vector<MeshInfos> order;
			std::bitset<MeshInfos::END> infos;

//			if (loadOrder.empty())
//			{
				for (std::size_t j = 0; i < data.subMeshs[i].infos.size(); ++j)
				{
					if (!data.subMeshs[i].infos.test(j))
						continue;
					order.push_back(MeshInfos((1 << j)));
					infos.set(j);
				}
//			}
//			else
//			{
//				for (auto &e : loadOrder)
//				{
//					if (!data.subMeshs[i].infos.test(e))
//						continue;
//					order.push_back(e);
//					infos.set(e);
//				}
//			}
			loadSubmesh(data.subMeshs[i], &meshInstance->subMeshs[i], /*order, */infos);
		}
		_meshs.insert(std::make_pair(filePath.getFullName(), meshInstance));
		return meshInstance;
	}

	void AssetsManager::loadSubmesh(SubMeshData &data
		, SubMeshInstance *mesh
//		, const std::vector<MeshInfos> &order
		, const std::bitset<MeshInfos::END> &infos)
	{

		std::size_t size = data.infos.count();

		AGE::Vector<void*> buffer;
		buffer.resize(size);
		AGE::Vector<std::size_t> nbrBuffer;
		nbrBuffer.resize(size);

		std::size_t loaded = 0;
		auto sizeofFloat = sizeof(float);
		auto maxSize = data.positions.size();

		for (uint32_t i = 0; i < infos.size(); ++i)
		{
			if (infos[i] == false)
				continue;
			switch (MeshInfos(i))
			{
			case Positions:
				buffer[loaded] = &data.positions;
				nbrBuffer[loaded] = data.positions.size() * 4 * sizeofFloat;
				if (data.positions.size() > maxSize)
					maxSize = data.positions.size();
				break;
			case Normals:
				buffer[loaded] = &data.normals;
				nbrBuffer[loaded] = data.normals.size() * 4 * sizeofFloat;
				if (data.normals.size() > maxSize)
					maxSize = data.normals.size();
				break;
			case Tangents:
				buffer[loaded] = &data.tangents;
				nbrBuffer[loaded] = data.tangents.size() * 4 * sizeofFloat;
				if (data.tangents.size() > maxSize)
					maxSize = data.tangents.size();
				break;
			case BiTangents:
				buffer[loaded] = &data.biTangents;
				nbrBuffer[loaded] = data.biTangents.size() * 4 * sizeofFloat;
				if (data.biTangents.size() > maxSize)
					maxSize = data.biTangents.size();
				break;
			case Uvs:
				buffer[loaded] = &data.uvs[0];
				nbrBuffer[loaded] = data.uvs[0].size() * 2 * sizeofFloat;
				if (data.uvs[0].size() > maxSize)
					maxSize = data.uvs[0].size();
				break;
			case Weights:
				buffer[loaded] = &data.weights;
				nbrBuffer[loaded] = data.weights.size() * 4 * sizeofFloat;
				if (data.weights.size() > maxSize)
					maxSize = data.weights.size();
				break;
			case BoneIndices:
				buffer[loaded] = &data.boneIndices;
				nbrBuffer[loaded] = data.boneIndices.size() * 4 * sizeofFloat;
				if (data.boneIndices.size() > maxSize)
					maxSize = data.boneIndices.size();
				break;
			case Colors:
				buffer[loaded] = &data.colors;
				nbrBuffer[loaded] = data.colors.size() * 4 * sizeofFloat;
				if (data.colors.size() > maxSize)
					maxSize = data.colors.size();
				break;
			default:
				break;
			}
			++loaded;
		}

		mesh->defaultMaterialIndex = data.defaultMaterialIndex;
		mesh->boundingBox = data.boundingBox;

		AGE::Vector<GLenum> types;

		for (uint32_t i = 0; i < infos.size(); ++i)
		{
			if (infos[i])
				types.push_back(g_InfosTypes[i]);
		}

		// create the painter
		auto future1 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=]()
		{
			if (_painters.find(infos) == _painters.end())
			{
				_painters[infos] = std::make_shared<Painter>(_programs, types);
			}
			return (true);
		});

		// fill the painter
		auto future2 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=]()
		{
			// We need to keep an instance of FileData shared_ptr
			std::shared_ptr<Painter> painter = _painters[infos];

			mesh->vertices = painter->add_vertices(3, 6);
			Vertices *v = painter->get_vertices(mesh->vertices);

			v->set_indices(data.indices);
			for (uint32_t attrib = 0; attrib < buffer.size(); ++attrib)
			{
				if (types[attrib] == GL_FLOAT_VEC4)
					v->set_data<glm::vec4>(*reinterpret_cast<AGE::Vector<glm::vec4>*>(buffer[attrib]), attrib);
				else if (types[attrib] == GL_FLOAT_VEC2)
					v->set_data<glm::vec2>(*reinterpret_cast<AGE::Vector<glm::vec2>*>(buffer[attrib]), attrib);
				else
					assert(!"Cannot load this type of datas");
			}
			return (true);
		});

		future1.get();
		future2.get();
	}
	
	std::shared_ptr<Program> AssetsManager::addProgram(std::string const &name, std::vector<std::shared_ptr<UnitProg>> const &u)
	{
		std::shared_ptr<Program> program = std::make_shared<Program>(std::string(name), u);

		_programs.push_back(program);
		_programsMap[name] = program;
		return (program);
	}

	std::shared_ptr<MeshInstance> AssetsManager::createMesh(std::string const &meshName,
															AGE::Vector<glm::vec4> const &positions,
															AGE::Vector<glm::vec4> const &colors,
															AGE::Vector<unsigned int> const &idx)
	{
		auto meshInstance = std::make_shared<MeshInstance>();

		meshInstance->subMeshs.resize(1);
		meshInstance->name = meshName;
		meshInstance->path = meshName;

		auto mesh = &meshInstance->subMeshs[0];

		std::bitset<MeshInfos::END> infos;

		infos[MeshInfos::Positions] = true;
		infos[MeshInfos::Colors] = true;

		// create the painter
		auto future1 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=]()
		{
			if (_painters.find(infos) == _painters.end())
			{
				_painters[infos] = std::make_shared<Painter>(Painter(_programs, { GL_FLOAT_VEC4, GL_FLOAT_VEC4 }));
			}
			return (true);
		});

		// fill the painter
		auto future2 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<AGE::Tasks::Basic::BoolFunction, bool>([=]()
		{
			// We need to keep an instance of FileData shared_ptr
			std::shared_ptr<Painter> painter = _painters[infos];

			mesh->vertices = painter->add_vertices(3, 6);
			Vertices *v = painter->get_vertices(mesh->vertices);

			v->set_indices(idx);
			v->set_data<glm::vec4>(positions, 0);
			v->set_data<glm::vec4>(colors, 1);
			return (true);
		});

		future1.get();
		future2.get();
		_meshs[meshName] = meshInstance;
		return (meshInstance);
	}

	// Create pool for mesh
	void AssetsManager::createPool(const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos)
	{

		std::size_t size = infos.count();

		AGE::Vector<GLenum> typeComponent(size); // {GL_FLOAT, GL_FLOAT, GL_FLOAT};			
		AGE::Vector<uint8_t> sizeTypeComponent(size); // { sizeof(float), sizeof(float), sizeof(float) };
		AGE::Vector<uint8_t> nbrComponent(size);// {4, 4, 4};

		std::size_t ctr = 0;

		for (auto &e : order)
		{
			switch (e)
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

	}
}