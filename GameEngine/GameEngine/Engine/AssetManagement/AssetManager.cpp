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
#include <Threads/MainThread.hpp>
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

#include <Configuration.hpp>

#ifdef USE_IMGUI
#include <imgui/imgui.h>
#endif

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

				auto normalTex = std::make_shared<MapColor>("normal");
				materialSubset._properties.push_back(normalTex);
				auto normalTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.normalTexPath, loadingChannel));
				normalTex->set(normalTexPtr);

				auto diffuseTex = std::make_shared<MapColor>("diffuse");
				materialSubset._properties.push_back(diffuseTex);
				auto diffuseTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.diffuseTexPath, loadingChannel));
				diffuseTex->set(diffuseTexPtr);

				auto ambientTex = std::make_shared<MapColor>("ambient");
				materialSubset._properties.push_back(ambientTex);
				auto ambientTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.ambientTexPath, loadingChannel));
				ambientTex->set(ambientTexPtr);

				auto emissiveTex = std::make_shared<MapColor>("emissive");
				materialSubset._properties.push_back(emissiveTex);
				auto emissiveTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.emissiveTexPath, loadingChannel));
				emissiveTex->set(emissiveTexPtr);

				auto reflectiveTex = std::make_shared<MapColor>("reflective");
				materialSubset._properties.push_back(reflectiveTex);
				auto reflectiveTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.reflectiveTexPath, loadingChannel));
				reflectiveTex->set(reflectiveTexPtr);

				auto specularTex = std::make_shared<MapColor>("specular");
				materialSubset._properties.push_back(specularTex);
				auto specularTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.specularTexPath, loadingChannel));
				specularTex->set(specularTexPtr);

//				auto futureSubMaterial = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<Tasks::Render::AddMaterial, MaterialInstance>(material_data);
//				auto subMaterial = futureSubMaterial.get();
//				material_set->datas.emplace_back(subMaterial);
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	std::shared_ptr<ITexture> AssetsManager::loadTexture(
		const OldFile &_filePath
		, const std::string &loadingChannel)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());

		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_textures.find(filePath.getFullName()) != std::end(_textures))
			{
				return _textures[filePath.getFullName()];
			}
		}

		if (!filePath.exists())
		{
			return nullptr;
		}

		auto texture = std::make_shared<Texture2D>();

		{
			std::lock_guard<std::mutex> lock(_mutex);
			std::shared_ptr<ITexture> textureInterface = std::static_pointer_cast<ITexture>(texture);
			_textures.insert(std::make_pair(filePath.getFullName(), textureInterface));
		}

		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			std::shared_ptr<TextureData> data = std::make_shared<TextureData>();

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
			auto success = texture->init(data->width, data->height, ct, true);
			if (success == false)
			{
				return AssetsLoadingResult(false, "Texture loading error");
			}
			texture->bind();
			texture->set(data->data, 0, color, GL_UNSIGNED_BYTE);
			texture->parameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			texture->parameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			switch (data->repeatX)
			{
			case TextureData::NoRepeat:
				break;
			case TextureData::Repeat:
				texture->parameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
				break;
			case TextureData::MirrorRepeat:
				texture->parameter(GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
				break;
			case TextureData::ClampToBorder:
				texture->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
				break;
			case TextureData::ClampToEdge:
				texture->parameter(GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				break;
			}

			switch (data->repeatY)
			{
			case TextureData::NoRepeat:
				break;
			case TextureData::Repeat:
				texture->parameter(GL_TEXTURE_WRAP_S, GL_REPEAT);
				break;
			case TextureData::MirrorRepeat:
				texture->parameter(GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
				break;
			case TextureData::ClampToBorder:
				texture->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
				break;
			case TextureData::ClampToEdge:
				texture->parameter(GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				break;
			}

			texture->generateMipmaps();
			texture->unbind();
			return AssetsLoadingResult(true);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return texture;
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

	bool AssetsManager::loadMesh(const OldFile &_filePath, const std::string &loadingChannel)
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
					loadSubmesh(data, i, &meshInstance->subMeshs[i], loadingChannel);
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, data->subMeshs[i].name, future);
			}
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	void AssetsManager::loadSubmesh(std::shared_ptr<MeshData> fileData, std::size_t index, SubMeshInstance *mesh, const std::string &loadingChannel)
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
			for (auto i = 0ull; i < data.infos.size(); ++i)
			{
				if (data.infos.test(i))
				{
					types.emplace_back(g_InfosTypes[i].first);
				}
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
			for (auto i = 0ull; i < data.infos.size(); ++i)
			{
				if (data.infos.test(i))
				{
					g_InfosTypes[i].second(*vertices, i, data);
				}
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
			}
			channel = _loadingChannels[loadingChannel];
		}
		channel->pushNewAsset(filename, future);
	}

	void AssetsManager::pushNewCallback(const std::string &loadingChannel, AScene *currentScene, std::function<void()> &callback)
	{
		std::shared_ptr<AssetsManager::AssetsLoadingChannel> channel = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_loadingChannels.find(loadingChannel) == std::end(_loadingChannels))
			{
				_loadingChannels.insert(std::make_pair(loadingChannel, std::make_shared<AssetsManager::AssetsLoadingChannel>()));
			}
			channel = _loadingChannels[loadingChannel];
		}
		channel->pushNewCallback(callback, currentScene);
	}

	bool AssetsManager::AssetsLoadingChannel::updateList(int &noLoaded, int &total)
	{
		std::lock_guard<std::mutex> lock(_mutex);
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
		noLoaded = (int)_list.size();
		total = (int)_maxAssets;
		return _errorMessages.empty();
	}

	void AssetsManager::AssetsLoadingChannel::callCallbacks()
	{
		for (auto &e : _callbacks)
		{
			if (e.callback && e.scene != nullptr)
			{
				GetMainThread()->setSceneAsActive(e.scene);
				e.callback();
			}
		}
		_callbacks.clear();
	}

	void AssetsManager::AssetsLoadingChannel::pushNewAsset(const std::string &filename, std::future<AssetsLoadingResult> &future)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_list.push_back(AssetsManager::AssetsLoadingStatus(filename, future));
		if (_list.size() > _maxAssets)
			_maxAssets = _list.size();
	}

	void AssetsManager::AssetsLoadingChannel::pushNewCallback(std::function<void()> &callback, AScene *scene)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		_callbacks.push_back(CallBackContainer());
		_callbacks.back().callback = callback;
		_callbacks.back().scene = scene;
	}

	// has to be called only once per frame
	void AssetsManager::update()
	{
		std::vector<std::string> toErase;
		int total = 0;
		int toLoad = 0;
		std::shared_ptr<AssetsManager::AssetsLoadingChannel> channel = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			auto it = std::begin(_loadingChannels);
			while (it != std::end(_loadingChannels))
			{
				channel = it->second;
				int channelToLoad = 0;
				int channelTotal = 0;
				auto success = channel->updateList(channelToLoad, channelTotal);
				if (!success)
				{
					AGE_ERROR(channel->getErrorMessages());
				}
				if (channelToLoad == 0)
				{
					toErase.push_back(it->first);
				}
				++it;
				total += channelTotal;
				toLoad += channelToLoad;
			}
		}

		for (auto &e : toErase)
		{
			_loadingChannels[e]->callCallbacks();
			_loadingChannels.erase(e);
		}

		if (toLoad != 0)
		{
#ifdef USE_IMGUI
			if (!ImGui::Begin("ASSETS LOADING", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings))
			{
				ImGui::End();
			}
			else
			{
				ImGui::SetWindowPos(ImVec2(30, 30));
				ImGui::Text("Assets loading : %s / %s", std::to_string(toLoad).c_str(), std::to_string(total).c_str());
				ImGui::End();
			}
#endif
			_isLoading = true;
		}
		else
		{
			_isLoading = false;
		}
	}

	bool AssetsManager::isLoading()
	{
		return _isLoading;
	}

	void AssetsManager::loadPackage(const OldFile &packagePath, const std::string &loadingChannel /*= ""*/)
	{
		if (!packagePath.exists())
		{
			return;
		}
		std::ifstream file(std::string(packagePath.getFullName()), std::ios::binary);
		AGE_ASSERT(file.is_open());
		{
			AssetsPackage package;
			auto ar = cereal::JSONInputArchive(file);
			ar(package);
			loadPackage(package, loadingChannel);
		}
	}

	void AssetsManager::loadPackage(const AssetsPackage &package, const std::string &loadingChannel /*= ""*/)
	{
		for (auto &e : package.meshs)
		{
			loadMesh(OldFile(e), loadingChannel);
		}
		for (auto &e : package.materials)
		{
			loadMaterial(OldFile(e), loadingChannel);
		}
	}

	void AssetsManager::savePackage(const AssetsPackage &package, const std::string filePath)
	{
		std::ofstream file(filePath, std::ios::binary);
		AGE_ASSERT(file.is_open());
		{
			auto ar = cereal::JSONOutputArchive(file);
			ar(package);
		}
	}

	std::shared_ptr<ITexture> const &AssetsManager::getPointLightTexture()
	{
		if (!_pointLight)
			_pointLight = loadTexture("pointlight.tage", "");
		return _pointLight;
	}
}