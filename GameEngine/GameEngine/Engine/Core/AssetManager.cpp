#include <Core/AssetsManager.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include <Data/Material/MaterialData.hh>
#include <Data/TextureData.hh>
#include <Core/Material/MaterialInstance.hh>
#include <Geometry/Mesh.hpp>
#include <Threads/ThreadManager.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Threads/QueueOwner.hpp>
#include <Threads/Thread.hpp>
#include <Render/Properties/Materials/Diffuse.hh>
#include <Render/Textures/ITexture.hh>

namespace AGE
{
	AssetsManager::AssetsManager()
	{
		GetThreadManager()->forEachThreads([](AGE::Thread *t) {
			dynamic_cast <QueueOwner*>(t)->registerCallback<LoadAssetMessage>([t](LoadAssetMessage &msg){
				msg.setValue(msg.function());
			});
		});
	}

	std::shared_ptr<MeshInstance> AssetsManager::getMesh(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<MaterialSetInstance> AssetsManager::getMaterial(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_materials.find(filePath.getFullName()) != std::end(_materials)) {
			return _materials[filePath.getFullName()];
		}
		return nullptr;
	}

	void AssetsManager::loadMaterial(const File &_filePath, const std::string &loadingChannel)
	{
		auto material = std::make_shared<MaterialSetInstance>();
		File filePath(_assetsDirectory + _filePath.getFullName());
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_materials.find(filePath.getFullName()) != std::end(_materials)) {
				return;
			}
			_materials.insert(std::make_pair(filePath.getFullName(), material));
		}
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			if (!filePath.exists()) {
				return AssetsLoadingResult(true, std::string("AssetsManager : Mesh File [" + filePath.getFullName() + "] does not exists.\n"));
			}
			std::shared_ptr<MaterialDataSet> material_data_set = std::make_shared<MaterialDataSet>();
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*material_data_set.get());
			material->name = material_data_set->name;
			material->path = _filePath.getFullName();
			auto i = 0;
			material->datas.resize(material_data_set->collection.size());
			for (auto &material_data : material_data_set->collection) {
				auto futureSubMaterial = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
				{
					material->datas[i].emplace_back(std::make_shared<Diffuse>());
					std::static_pointer_cast<Diffuse>(material->datas[i].back())->set_color(material_data.diffuse);
					std::static_pointer_cast<Diffuse>(material->datas[i].back())->set_ratio(1.0f);
					std::static_pointer_cast<Diffuse>(material->datas[i].back())->set_map(nullptr);
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, _filePath.getFullName() + std::to_string(i), futureSubMaterial);
				++i;
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
	}

	void AssetsManager::loadTexture(const File &_filePath, const std::string &loadingChannel, std::function<void(ITexture &key_tex)> &callback)
	{
		std::shared_ptr<TextureData> data = std::make_shared<TextureData>();
		File filePath(_assetsDirectory + _filePath.getFullName());

		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				if (_textures.find(filePath.getFullName()) != std::end(_textures))
				{
					auto texture = _textures[filePath.getFullName()];
					callback(*texture.get());
					return AssetsLoadingResult(texture == nullptr);
				}
			}

			if (!filePath.exists())
			{
				auto key = manager->getDefaultTexture2D();
				callback(key);
				_textures.insert(std::make_pair(filePath.getFullName(), std::make_shared<gl::Key<gl::Texture>>(key)));
				return AssetsLoadingResult(key.empty());
			}

			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*data.get());

			// TODO fill texture with texture key

			GLenum ct = GL_RGB32F;
			GLenum color = GL_RGB;
			if (data->colorNumber == 3)
			{
				ct = /*GL_COMPRESSED_RGB_S3TC_DXT1_EXT;//*/GL_RGB32F;
				color = GL_BGR;
			}
			else if (data->colorNumber == 4)
			{
				ct = /*GL_COMPRESSED_RGBA_S3TC_DXT5_EXT;//*/ GL_RGBA32F;
				color = GL_BGRA;
			}
			else if (data->colorNumber == 1)
			{
				ct = /*GL_COMPRESSED_RGB_S3TC_DXT1_EXT;//*/ GL_RGB32F;
				color = GL_LUMINANCE;
			}
			else
				return AssetsLoadingResult(true, "Image format not found.\n");
			auto key = manager->addTexture2D(data->width, data->height, ct, true);
			
			manager->uploadTexture(key, color, GL_UNSIGNED_BYTE, data->data.data());
			manager->parameterTexture(key, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			manager->parameterTexture(key, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			manager->parameterTexture(key, GL_TEXTURE_WRAP_T, GL_REPEAT);
			manager->parameterTexture(key, GL_TEXTURE_WRAP_S, GL_REPEAT);
			
			callback(key);
				_textures.insert(std::make_pair(filePath.getFullName(), std::make_shared<gl::Key<gl::Texture>>(key)));
			return AssetsLoadingResult(key.empty());
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
	}

	void AssetsManager::loadAnimation(const File &_filePath, const std::string &loadingChannel)
>>>>>>> master
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		auto animation = std::make_shared<Animation>();
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_animations.find(filePath.getFullName()) != std::end(_animations))
			{
				return;
			}
			_animations.insert(std::make_pair(filePath.getFullName(), animation));
		}

		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=](){
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*animation.get());
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
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

	void AssetsManager::loadSkeleton(const File &_filePath, const std::string &loadingChannel)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		auto skeleton = std::make_shared<Skeleton>();
		if (!filePath.exists())
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			assert(false);
		}

<<<<<<< HEAD
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
=======
>>>>>>> master
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			{
				return;
			}
			_skeletons.insert(std::make_pair(filePath.getFullName(), skeleton));
		}
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=](){
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*skeleton.get());
			return true;
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
	}

<<<<<<< HEAD
		std::ifstream ifs(filePath.getFullName(), std::ios::binary);
		cereal::PortableBinaryInputArchive ar(ifs);
		MeshData data;
		ar(data);

//		auto res = AGE::GetRenderThread()->getQueue()->

=======
	void AssetsManager::loadMesh(const File &_filePath, const std::vector<MeshInfos> &loadOrder, const std::string &loadingChannel)
	{
>>>>>>> master
		auto meshInstance = std::make_shared<MeshInstance>();
		File filePath(_assetsDirectory + _filePath.getFullName());
		{
<<<<<<< HEAD
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
=======
			std::lock_guard<std::mutex> lock(_mutex);
			if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			{
				return;
			}
			_meshs.insert(std::make_pair(filePath.getFullName(), meshInstance));
		}

		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=](){
			if (!filePath.exists())
			{
				return AssetsLoadingResult(true, std::string("AssetsManager : Mesh File [" + filePath.getFullName() + "] does not exists.\n"));
			}
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			std::shared_ptr<MeshData> data = std::make_shared<MeshData>();
			ar(*data.get());

			meshInstance->subMeshs.resize(data->subMeshs.size());
			meshInstance->name = data->name;
			meshInstance->path = _filePath.getFullName();

			for (std::size_t i = 0; i < data->subMeshs.size(); ++i)
			{
				auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=](){
					// If no vertex pool correspond to submesh
					std::vector<MeshInfos> order;
					std::bitset<MeshInfos::END> infos;
					if (loadOrder.empty())
					{
						for (std::size_t j = 0; i < data->subMeshs[i].infos.size(); ++j)
						{
							if (!data->subMeshs[i].infos.test(j))
								continue;
							order.push_back(MeshInfos(j));
							infos.set(j);
						}
					}
					else
					{
						for (auto &e : loadOrder)
						{
							if (!data->subMeshs[i].infos.test(e))
								continue;
							order.push_back(e);
							infos.set(e);
						}
					}
					loadSubmesh(data, i, &meshInstance->subMeshs[i], order, infos, loadingChannel);
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, data->subMeshs[i].name, future);
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
	}

	void AssetsManager::loadSubmesh(std::shared_ptr<MeshData> fileData
		, std::size_t index
		, SubMeshInstance *mesh
		, const std::vector<MeshInfos> &order
		, const std::bitset<MeshInfos::END> &infos
		, const std::string &loadingChannel)
	{
		auto m = _dependencyManager.lock()->getInstance<gl::RenderManager>();
>>>>>>> master

		auto &data = fileData->subMeshs[index];

		std::size_t size = data.infos.count();

		std::shared_ptr<AGE::Vector<void*>> buffer = std::make_shared<AGE::Vector<void*>>();
		buffer->resize(size);
		std::shared_ptr<AGE::Vector<std::size_t>> nbrBuffer = std::make_shared<AGE::Vector<std::size_t>>();
		nbrBuffer->resize(size);

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
<<<<<<< HEAD
				buffer[loaded] = &data.positions;
				nbrBuffer[loaded] = data.positions.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.positions[0].x;
				(*nbrBuffer)[ctr] = data.positions.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.positions.size() > maxSize)
					maxSize = data.positions.size();
				break;
			case Normals:
<<<<<<< HEAD
				buffer[loaded] = &data.normals;
				nbrBuffer[loaded] = data.normals.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.normals[0].x;
				(*nbrBuffer)[ctr] = data.normals.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.normals.size() > maxSize)
					maxSize = data.normals.size();
				break;
			case Tangents:
<<<<<<< HEAD
				buffer[loaded] = &data.tangents;
				nbrBuffer[loaded] = data.tangents.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.tangents[0].x;
				(*nbrBuffer)[ctr] = data.tangents.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.tangents.size() > maxSize)
					maxSize = data.tangents.size();
				break;
			case BiTangents:
<<<<<<< HEAD
				buffer[loaded] = &data.biTangents;
				nbrBuffer[loaded] = data.biTangents.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.biTangents[0].x;
				(*nbrBuffer)[ctr] = data.biTangents.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.biTangents.size() > maxSize)
					maxSize = data.biTangents.size();
				break;
			case Uvs:
<<<<<<< HEAD
				buffer[loaded] = &data.uvs[0];
				nbrBuffer[loaded] = data.uvs[0].size() * 2 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.uvs[0][0].x;
				(*nbrBuffer)[ctr] = data.uvs[0].size() * 2 * sizeofFloat;
>>>>>>> master
				if (data.uvs[0].size() > maxSize)
					maxSize = data.uvs[0].size();
				break;
			case Weights:
<<<<<<< HEAD
				buffer[loaded] = &data.weights;
				nbrBuffer[loaded] = data.weights.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.weights[0].x;
				(*nbrBuffer)[ctr] = data.weights.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.weights.size() > maxSize)
					maxSize = data.weights.size();
				break;
			case BoneIndices:
<<<<<<< HEAD
				buffer[loaded] = &data.boneIndices;
				nbrBuffer[loaded] = data.boneIndices.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.boneIndices[0].x;
				(*nbrBuffer)[ctr] = data.boneIndices.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.boneIndices.size() > maxSize)
					maxSize = data.boneIndices.size();
				break;
			case Colors:
<<<<<<< HEAD
				buffer[loaded] = &data.colors;
				nbrBuffer[loaded] = data.colors.size() * 4 * sizeofFloat;
=======
				(*buffer)[ctr] = &data.colors[0].x;
				(*nbrBuffer)[ctr] = data.colors.size() * 4 * sizeofFloat;
>>>>>>> master
				if (data.colors.size() > maxSize)
					maxSize = data.colors.size();
				break;
			default:
				break;
			}
			++loaded;
		}

<<<<<<< HEAD
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

			mesh->vertices = painter->add_vertices(positions.size(), idx.size());
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
=======
		mesh->boundingBox = data.boundingBox;
		mesh->defaultMaterialIndex = data.defaultMaterialIndex;
		auto future1 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			if (_pools.find(infos) == std::end(_pools))
			{
				createPool(order, infos);
			}
			return AssetsLoadingResult(false);
		});
		auto future2 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			// We need to keep an instance of FileData shared_ptr
			auto fileDataCopy = fileData;
			(void)(fileDataCopy);
			auto &pools = _pools.find(infos)->second;
			mesh->vertices = m->addVertices(maxSize, std::cref(*(nbrBuffer.get())), std::cref(*(buffer.get())), pools.first);
			mesh->indices = m->addIndices(data.indices.size(), data.indices, pools.second);
			mesh->vertexPool = pools.first;
			mesh->indexPool = pools.second;
			return AssetsLoadingResult(false);
		});
		auto future3 = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			// We need to keep an instance of FileData shared_ptr
			auto fileDataCopy = fileData;
			(void)(fileDataCopy);
			auto &pools = _pools.find(infos)->second;
			mesh->indices = m->addIndices(data.indices.size(), data.indices, pools.second);
			mesh->vertexPool = pools.first;
			mesh->indexPool = pools.second;
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, data.name, future1);
		pushNewAsset(loadingChannel, data.name, future2);
		pushNewAsset(loadingChannel, data.name, future3);
>>>>>>> master
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

	void AssetsManager::pushNewAsset(const std::string &loadingChannel, const std::string &filename, std::future<AssetsManager::AssetsLoadingResult> &future)
	{
		std::shared_ptr<AssetsManager::AssetsLoadingChannel> channel = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_loadingChannels.find(loadingChannel) == std::end(_loadingChannels))
			{
				_loadingChannels.insert(std::make_pair(loadingChannel, std::make_shared<AssetsManager::AssetsLoadingChannel>()));
				_loadingChannels[loadingChannel]->_lastUpdate = std::chrono::high_resolution_clock::now();
			}
			channel = _loadingChannels[loadingChannel];
		}
		channel->pushNewAsset(filename, future);
	}

	bool AssetsManager::AssetsLoadingChannel::updateList(std::size_t &noLoaded, std::size_t &total)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		if (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - _lastUpdate).count() > 33)
		{
			_lastUpdate = std::chrono::high_resolution_clock::now();
			std::size_t i = 0;
			_list.remove_if([&](AssetsManager::AssetsLoadingStatus &e){
				if (i > 30)
					return false;
				++i;
				if (!e.future.valid())
				{
					_errorMessages += "ERROR : Future is invalid !\n";
					return true;
				}
				auto status = e.future.wait_for(std::chrono::microseconds(10));
				if (status == std::future_status::ready)
				{
					e.result = e.future.get();
					if (e.result.error)
					{
						_errorMessages += e.result.errorMessage;
					}
					return true;
				}
				return false;
			});
		}
		noLoaded = _list.size();
		total = _maxAssets;
		return _errorMessages.empty();
	}

	void AssetsManager::AssetsLoadingChannel::pushNewAsset(const std::string &filename, std::future<AssetsLoadingResult> &future)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_list.push_back(AssetsManager::AssetsLoadingStatus(filename, future));
		if (_list.size() > _maxAssets)
			_maxAssets = _list.size();
	}

	void AssetsManager::updateLoadingChannel(const std::string &channelName, std::size_t &total, std::size_t &to_load, std::string &error)
	{
		std::shared_ptr<AssetsManager::AssetsLoadingChannel> channel = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_loadingChannels.find(channelName) == std::end(_loadingChannels))
			{
				total = -1;
				to_load = -1;
				return;
			}
			channel = _loadingChannels[channelName];
		}
		if (!channel->updateList(to_load, total))
			error = channel->getErrorMessages();
	}
}