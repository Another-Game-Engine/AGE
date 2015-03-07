#include <fstream>

#include <AssetManagement/AssetManager.hh>
#include <Skinning/Skeleton.hpp>
#include <AssetManagement/Data/AnimationData.hpp>
#include <AssetManagement/Data/MaterialData.hh>
#include <AssetManagement/Data/MeshData.hh>
#include <AssetManagement/Data/TextureData.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <Threads/ThreadManager.hpp>
#include <Threads/Tasks/BasicTasks.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Threads/QueueOwner.hpp>
#include <Threads/Thread.hpp>
#include <Threads/Tasks/ToRenderTasks.hpp>
#include <Render/Properties/Materials/Diffuse.hh>
#include <Render/Textures/Texture2D.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>

#include <Render/Properties/Transformation.hh>
#include <Render/Properties/Materials/Color.hh>
#include <Render/Properties/Materials/Diffuse.hh>
#include <Render/Properties/Materials/MapColor.hh>
#include <Render/Properties/Materials/Specular.hh>
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

	std::shared_ptr<MeshInstance> AssetsManager::getMesh(const OldFile &_filePath)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		std::lock_guard<std::mutex> lock(_mutex);

		if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			return _meshs[filePath.getFullName()];
		return nullptr;
	}

	std::shared_ptr<MaterialSetInstance> AssetsManager::getMaterial(const OldFile &_filePath)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		std::lock_guard<std::mutex> lock(_mutex);

		if (_materials.find(filePath.getFullName()) != std::end(_materials)) 
		{
			return _materials[filePath.getFullName()];
		}
		return nullptr;
	}

	bool AssetsManager::loadMaterial(const OldFile &_filePath, const std::string &loadingChannel)
	{
		auto material = std::make_shared<MaterialSetInstance>();
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_materials.find(filePath.getFullName()) != std::end(_materials))
			{
				return (true);
			}
			_materials.insert(std::make_pair(filePath.getFullName(), material));
		}
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			if (!filePath.exists()) 
			{
				return AssetsLoadingResult(true, std::string("AssetsManager : Mesh File [" + filePath.getFullName() + "] does not exists.\n"));
			}
			std::shared_ptr<MaterialDataSet> material_data_set = std::make_shared<MaterialDataSet>();
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*material_data_set.get());
			material->name = material_data_set->name;
			material->path = _filePath.getFullName();
			for (auto &material_data : material_data_set->collection) 
			{
//#include <Render/Properties/Materials/Color.hh>
//#include <Render/Properties/Materials/Diffuse.hh>
//#include <Render/Properties/Materials/MapColor.hh>
//#include <Render/Properties/Materials/Specular.hh>
				material->datas.push_back(MaterialInstance());
				auto &materialSubset = material->datas.back();

				auto diffuse = std::make_shared<Color>("diffuse");
				materialSubset._properties.push_back(diffuse);
				diffuse->set(material_data.diffuse);

				auto ambient = std::make_shared<Color>("ambient");
				materialSubset._properties.push_back(ambient);
				ambient->set(material_data.ambient);

				auto emissive = std::make_shared<Color>("emissive");
				materialSubset._properties.push_back(emissive);
				emissive->set(material_data.emissive);

				auto reflective = std::make_shared<Color>("reflective");
				materialSubset._properties.push_back(reflective);
				reflective->set(material_data.reflective);

				auto specular = std::make_shared<Color>("specular");
				materialSubset._properties.push_back(specular);
				specular->set(material_data.specular);

//				auto futureSubMaterial = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<Tasks::Render::AddMaterial, MaterialInstance>(material_data);
//				auto subMaterial = futureSubMaterial.get();
//				material_set->datas.emplace_back(subMaterial);
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	bool AssetsManager::loadTexture(const OldFile &_filePath, const std::string &loadingChannel, std::function<void(std::shared_ptr<ITexture> &texture)> &callback)
	{
		std::shared_ptr<TextureData> data = std::make_shared<TextureData>();
		OldFile filePath(_assetsDirectory + _filePath.getFullName());

		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			{
				std::lock_guard<std::mutex> lock(_mutex);
				if (_textures.find(filePath.getFullName()) != std::end(_textures)) 
				{
					return AssetsLoadingResult(true);
				}
			}
			if (!filePath.exists()) 
			{
				return AssetsLoadingResult(false);
			}
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*data.get());
			GLenum ct = GL_RGB32F;
			GLenum color = GL_RGB;
			switch (data->colorNumber)
			{
			case 3:
				ct = GL_RGB32F;
				color = GL_BGR;
				break;
			case 4:
				ct =  GL_RGBA32F;
				color = GL_BGRA;
				break;
			case 1:
				ct = GL_RGB32F;
				color = GL_LUMINANCE;
				break;
			default:
				return AssetsLoadingResult(true, "Image format not found.\n");
				break;
			}
			auto texture = std::make_shared<Texture2D>(data->width, data->height, ct, true);
			texture->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			texture->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			texture->parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
			texture->parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
			texture->set(data->data, 0, color, GL_UNSIGNED_BYTE);
			std::shared_ptr<ITexture> textureInterface = std::static_pointer_cast<ITexture>(texture);
			callback(textureInterface);
			_textures.insert(std::make_pair(filePath.getFullName(), textureInterface));
			return AssetsLoadingResult(true);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	bool AssetsManager::loadAnimation(const OldFile &_filePath, const std::string &loadingChannel)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		auto animation = std::make_shared<AnimationData>();
		if (!filePath.exists()) 
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			return (false);
		}
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_animations.find(filePath.getFullName()) != std::end(_animations)) 
			{
				return (true);
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
		return (true);
	}

	std::shared_ptr<AnimationData> AssetsManager::getAnimation(const OldFile &_filePath)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		return nullptr;
	}

	bool AssetsManager::loadSkeleton(const OldFile &_filePath, const std::string &loadingChannel)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		auto skeleton = std::make_shared<Skeleton>();
		if (!filePath.exists()) 
		{
			std::cerr << "AssetsManager : File [" << filePath.getFullName() << "] does not exists." << std::endl;
			return (false);
		}
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons)) 
			{
				return (true);
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
		return (true);
	}


	std::shared_ptr<Skeleton> AssetsManager::getSkeleton(const OldFile &_filePath)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		return nullptr;
	}

	bool AssetsManager::loadMesh(const OldFile &_filePath, const std::vector<MeshInfos> &loadOrder, const std::string &loadingChannel)
	{
		auto meshInstance = std::make_shared<MeshInstance>();
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			{
				return (true);
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
			// If no vertex pool correspond to submesh
			for (std::size_t i = 0; i < data->subMeshs.size(); ++i)
			{
				auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=](){
					loadSubmesh(data, i, &meshInstance->subMeshs[i], loadOrder, loadingChannel);
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, data->subMeshs[i].name, future);
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	void AssetsManager::loadSubmesh(std::shared_ptr<MeshData> fileData, std::size_t index, SubMeshInstance *mesh, const std::vector<MeshInfos> loadOrder, const std::string &loadingChannel)
	{
		auto &data = fileData->subMeshs[index];
		std::size_t size = data.infos.count();
		auto buffers = std::make_shared<AGE::Vector<std::vector<uint8_t>>>();
		buffers->resize(size);
		std::size_t ctr = 0;
		auto sizeofFloat = sizeof(float);
		auto maxSize = data.positions.size();
		mesh->boundingBox = data.boundingBox;
		mesh->defaultMaterialIndex = data.defaultMaterialIndex;
		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() {
			auto &paintingManager = GetRenderThread()->paintingManager;
			std::vector<std::pair<GLenum, std::string>> types;
			for (auto &e : loadOrder)
			{
				if (!fileData->subMeshs[index].infos.test(e))
					continue;
				types.emplace_back(g_InfosTypes[e].first);
			}
			if (!paintingManager->has_painter(types))
			{
				mesh->painter = paintingManager->add_painter(std::move(types));
			}
			else
			{
				mesh->painter = paintingManager->get_painter(types);
			}
			auto &painter = paintingManager->get_painter(mesh->painter);
			mesh->vertices = painter->add_vertices(data.positions.size(), data.indices.size());
			auto vertices = painter->get_vertices(mesh->vertices);
			for (auto i = 0ull; i < vertices->nbr_buffer(); ++i)
			{
				g_InfosTypes[loadOrder[i]].second(*vertices, i, data);
			}
			vertices->set_indices(data.indices);
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, data.name, future);
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