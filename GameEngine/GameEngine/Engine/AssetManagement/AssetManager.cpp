#include <AssetManagement/AssetManager.hh>
#include <Skinning/Skeleton.hpp>
#include <Skinning/Animation.hpp>
#include <AssetManagement/Data/MaterialData.hh>
#include <AssetManagement/Data/MeshData.hh>
#include <AssetManagement/Data/TextureData.hh>
#include <AssetManagement/Instance/MaterialInstance.hh>
#include <AssetManagement/Instance/MeshInstance.hh>
#include <Threads/ThreadManager.hpp>
#include <Core/Tasks/Basics.hpp>
#include <Threads/RenderThread.hpp>
#include <Threads/TaskScheduler.hpp>
#include <Threads/QueueOwner.hpp>
#include <Threads/Thread.hpp>
#include <Render/Properties/Materials/Diffuse.hh>
#include <Render/Textures/Texture2D.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>

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
		if (_materials.find(filePath.getFullName()) != std::end(_materials)) 
		{
			return _materials[filePath.getFullName()];
		}
		return nullptr;
	}

	bool AssetsManager::loadMaterial(const File &_filePath, const std::string &loadingChannel)
	{
		auto material = std::make_shared<MaterialSetInstance>();
		File filePath(_assetsDirectory + _filePath.getFullName());
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
			auto i = 0;
			material->datas.resize(material_data_set->collection.size());
			for (auto &material_data : material_data_set->collection) 
			{
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
		return (true);
	}

	bool AssetsManager::loadTexture(const File &_filePath, const std::string &loadingChannel)
	{
		std::shared_ptr<TextureData> data = std::make_shared<TextureData>();
		File filePath(_assetsDirectory + _filePath.getFullName());

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
			_textures.insert(std::make_pair(filePath.getFullName(), texture));
			return AssetsLoadingResult(true);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	bool AssetsManager::loadAnimation(const File &_filePath, const std::string &loadingChannel)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		auto animation = std::make_shared<Animation>();
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

	std::shared_ptr<Animation> AssetsManager::getAnimation(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		return nullptr;
	}

	bool AssetsManager::loadSkeleton(const File &_filePath, const std::string &loadingChannel)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
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


	std::shared_ptr<Skeleton> AssetsManager::getSkeleton(const File &_filePath)
	{
		File filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		return nullptr;
	}

	bool AssetsManager::loadMesh(const File &_filePath, const std::vector<MeshInfos> &loadOrder, const std::string &loadingChannel)
	{
		auto meshInstance = std::make_shared<MeshInstance>();
		File filePath(_assetsDirectory + _filePath.getFullName());
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
					loadSubmesh(data, i, meshInstance->subMeshs[i], loadOrder, loadingChannel);
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, data->subMeshs[i].name, future);
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	void AssetsManager::loadSubmesh(std::shared_ptr<MeshData> fileData, std::size_t index, SubMeshInstance &mesh, const std::vector<MeshInfos> &loadOrder, const std::string &loadingChannel)
	{
		auto &data = fileData->subMeshs[index];
		std::size_t size = data.infos.count();
		auto buffers = std::make_shared<AGE::Vector<std::vector<uint8_t>>>();
		buffers->resize(size);
		std::size_t ctr = 0;
		auto sizeofFloat = sizeof(float);
		auto maxSize = data.positions.size();
		mesh.boundingBox = data.boundingBox;
		mesh.defaultMaterialIndex = data.defaultMaterialIndex;
		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([&]() {
			auto &paintingManager = GetRenderThread()->paintingManager;
			std::vector<GLenum> types;
			for (auto &e : loadOrder)
			{
				if (!fileData->subMeshs[index].infos.test(e))
					continue;
				types.emplace_back(g_InfosTypes[e].first);
			}
			if (!paintingManager.has_painter(types))
			{
				mesh.painter = paintingManager.add_painter(std::move(types));
			}
			else
			{
				mesh.painter = paintingManager.get_painter(types);
			}
			auto &painter = paintingManager.get_painter(mesh.painter);
			mesh.vertices = painter->add_vertices(data.positions.size(), data.indices.size());
			auto vertices = painter->get_vertices(mesh.vertices);
			for (auto index = 0ull; vertices->nbr_buffer(); ++index)
			{
				g_InfosTypes[types[index]].second(*vertices, index, data);
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