#include <Core/AssetsManager.hpp>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include <Geometry/Mesh.hpp>
#include <Geometry/MaterialData.hpp>
#include <Texture/Texture.hpp>
#include <Render/RenderManager.hh>
#include <Threads/ThreadManager.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Threads/QueueOwner.hpp>
#include <Threads/Thread.hpp>
#include <Utils/Serialization/SerializationArchives.hpp>
#include <istream>
#include <fstream>

namespace AGE
{
	AssetsManager::AssetsManager()
	{
		//		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=](){
		GetThreadManager()->forEachThreads([](AGE::Thread *t){
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
		if (_materials.find(filePath.getFullName()) != std::end(_materials))
			return _materials[filePath.getFullName()];
		return nullptr;
	}

	void AssetsManager::loadMaterial(const File &_filePath, const std::string &loadingChannel)
	{
		auto material = std::make_shared<MaterialSetInstance>();
		File filePath(_assetsDirectory + _filePath.getFullName());
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_materials.find(filePath.getFullName()) != std::end(_materials))
			{
				return;
			}
			_materials.insert(std::make_pair(filePath.getFullName(), material));
		}

		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			if (!filePath.exists())
			{
				return AssetsLoadingResult(true, std::string("AssetsManager : Mesh File [" + filePath.getFullName() + "] does not exists.\n"));
			}

			std::shared_ptr<MaterialDataSet> data = std::make_shared<MaterialDataSet>();
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*data.get());
			material->name = data->name;
			material->path = _filePath.getFullName();
			auto manager = _dependencyManager.lock()->getInstance<gl::RenderManager>();
			int i = 0;
			material->datas.resize(data->collection.size());
			for (auto &e : data->collection)
			{
				auto futureSubMaterial = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
				{
					auto key = manager->addMaterial();
					material->datas[i] = key;

					// TODO fill material with material key
					gl::Key<gl::Material> &mat = material->datas[i];
					manager->setMaterial<gl::Color_diffuse>(mat, e.diffuse);
					manager->setMaterial<gl::Color_emissive>(mat, e.emissive);
					manager->setMaterial<gl::Color_specular>(mat, e.specular);
					manager->setMaterial<gl::Ratio_diffuse>(mat, 1.0f); // todo
					manager->setMaterial<gl::Ratio_emissive>(mat, 1.0f); // todo
					manager->setMaterial<gl::Ratio_specular>(mat, 1.0f); // todo
					loadTexture(e.diffuseTexPath, loadingChannel, std::function<void(gl::Key<gl::Texture> &)>([=](gl::Key<gl::Texture> &t){
						manager->setMaterial<gl::Texture_diffuse>(mat, t);
					}));
					loadTexture(e.emissiveTexPath, loadingChannel, std::function<void(gl::Key<gl::Texture> &)>([=](gl::Key<gl::Texture> &t){
						manager->setMaterial<gl::Texture_emissive>(mat, t);
					}));
					loadTexture(e.specularTexPath, loadingChannel, std::function<void(gl::Key<gl::Texture> &)>([=](gl::Key<gl::Texture> &t){
						manager->setMaterial<gl::Texture_specular>(mat, t);
					}));
					loadTexture(e.bumpTexPath, loadingChannel, std::function<void(gl::Key<gl::Texture> &)>([=](gl::Key<gl::Texture> &t){
						manager->setMaterial<gl::Texture_bump>(mat, t);
					}));
					loadTexture(e.normalTexPath, loadingChannel, std::function<void(gl::Key<gl::Texture> &)>([=](gl::Key<gl::Texture> &t){
						manager->setMaterial<gl::Texture_normal>(mat, t);
					}));
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, _filePath.getFullName() + std::to_string(i), futureSubMaterial);
				++i;
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
	}

	void AssetsManager::loadTexture(const File &_filePath, const std::string &loadingChannel, std::function<void(gl::Key<gl::Texture> &key_tex)> &callback)
	{
		auto manager = _dependencyManager.lock()->getInstance<gl::RenderManager>();
		std::shared_ptr<TextureData> data = std::make_shared<TextureData>();
		File filePath(_assetsDirectory + _filePath.getFullName());

		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				if (_textures.find(filePath.getFullName()) != std::end(_textures))
				{
					auto key = _textures[filePath.getFullName()];
					callback(*key.get());
					return AssetsLoadingResult(key->empty());
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

	void AssetsManager::loadMesh(const File &_filePath, const std::vector<MeshInfos> &loadOrder, const std::string &loadingChannel)
	{
		auto meshInstance = std::make_shared<MeshInstance>();
		File filePath(_assetsDirectory + _filePath.getFullName());
		{
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

		auto &data = fileData->subMeshs[index];

		std::size_t size = data.infos.count();

		std::shared_ptr<AGE::Vector<void*>> buffer = std::make_shared<AGE::Vector<void*>>();
		buffer->resize(size);
		std::shared_ptr<AGE::Vector<std::size_t>> nbrBuffer = std::make_shared<AGE::Vector<std::size_t>>();
		nbrBuffer->resize(size);

		std::size_t ctr = 0;
		auto sizeofFloat = sizeof(float);
		auto maxSize = data.positions.size();

		for (auto &e : order)
		{
			switch (MeshInfos(e))
			{
			case Positions:
				(*buffer)[ctr] = &data.positions[0].x;
				(*nbrBuffer)[ctr] = data.positions.size() * 4 * sizeofFloat;
				if (data.positions.size() > maxSize)
					maxSize = data.positions.size();
				break;
			case Normals:
				(*buffer)[ctr] = &data.normals[0].x;
				(*nbrBuffer)[ctr] = data.normals.size() * 4 * sizeofFloat;
				if (data.normals.size() > maxSize)
					maxSize = data.normals.size();
				break;
			case Tangents:
				(*buffer)[ctr] = &data.tangents[0].x;
				(*nbrBuffer)[ctr] = data.tangents.size() * 4 * sizeofFloat;
				if (data.tangents.size() > maxSize)
					maxSize = data.tangents.size();
				break;
			case BiTangents:
				(*buffer)[ctr] = &data.biTangents[0].x;
				(*nbrBuffer)[ctr] = data.biTangents.size() * 4 * sizeofFloat;
				if (data.biTangents.size() > maxSize)
					maxSize = data.biTangents.size();
				break;
			case Uvs:
				(*buffer)[ctr] = &data.uvs[0][0].x;
				(*nbrBuffer)[ctr] = data.uvs[0].size() * 2 * sizeofFloat;
				if (data.uvs[0].size() > maxSize)
					maxSize = data.uvs[0].size();
				break;
			case Weights:
				(*buffer)[ctr] = &data.weights[0].x;
				(*nbrBuffer)[ctr] = data.weights.size() * 4 * sizeofFloat;
				if (data.weights.size() > maxSize)
					maxSize = data.weights.size();
				break;
			case BoneIndices:
				(*buffer)[ctr] = &data.boneIndices[0].x;
				(*nbrBuffer)[ctr] = data.boneIndices.size() * 4 * sizeofFloat;
				if (data.boneIndices.size() > maxSize)
					maxSize = data.boneIndices.size();
				break;
			case Colors:
				(*buffer)[ctr] = &data.colors[0].x;
				(*nbrBuffer)[ctr] = data.colors.size() * 4 * sizeofFloat;
				if (data.colors.size() > maxSize)
					maxSize = data.colors.size();
				break;
			default:
				break;
			}
			++ctr;
		}

		mesh->boundingBox = data.boundingBox;
		mesh->defaultMaterialIndex = data.defaultMaterialIndex;
		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			if (_pools.find(infos) == std::end(_pools))
			{
				createPool(order, infos);
			}
		{
			// We need to keep an instance of FileData shared_ptr
			auto fileDataCopy = fileData;
			(void)(fileDataCopy);
			auto &pools = _pools.find(infos)->second;
			mesh->vertices = m->addVertices(maxSize, std::cref(*(nbrBuffer.get())), std::cref(*(buffer.get())), pools.first);
			mesh->indices = m->addIndices(data.indices.size(), data.indices, pools.second);
			mesh->vertexPool = pools.first;
			mesh->indexPool = pools.second;
		}
		{
			// We need to keep an instance of FileData shared_ptr
			auto fileDataCopy = fileData;
			(void)(fileDataCopy);
			auto &pools = _pools.find(infos)->second;
			mesh->indices = m->addIndices(data.indices.size(), data.indices, pools.second);
			mesh->vertexPool = pools.first;
			mesh->indexPool = pools.second;
		}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, data.name, future);
	}

	// Create pool for mesh
	void AssetsManager::createPool(const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos)
	{
		auto m = _dependencyManager.lock()->getInstance<gl::RenderManager>();
		assert(m != nullptr);

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

		auto vpKey = m->addVertexPool(uint8_t(size), typeComponent, sizeTypeComponent, nbrComponent);
		auto indKey = m->addIndexPool();


		_pools.insert(std::make_pair(infos, std::make_pair(vpKey, indKey)));
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