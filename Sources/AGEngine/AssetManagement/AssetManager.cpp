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
#include <Threads/RenderThread.hpp>

// TODO : @FASTBUILD @FBUILD passer le painer manager en Singleton, le detacher du render thread

#include <Render/Textures/Texture2D.hh>
#include <Render/GeometryManagement/Painting/PaintingManager.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>

#include <AssetManagement/OpenGLDDSLoader.hh>

#include <Utils/Profiler.hpp>

#include <Configuration.hpp>

#ifdef AGE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif

# define LAMBDA_FUNCTION [](AGE::Vertices &vertices, size_t index, AGE::SubMeshData const &data)

static std::pair<std::pair<GLenum, StringID>, std::function<void(AGE::Vertices &vertices, size_t index, AGE::SubMeshData const &data)>> g_InfosTypes[AGE::MeshInfos::END] =
{
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, StringID("position", 0x4cbf3a26fca1d74a)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.positions,      StringID("position", 0x4cbf3a26fca1d74a)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, StringID("normal", 0x61053f0e3ebbd272)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.normals,          StringID("normal", 0x61053f0e3ebbd272)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, StringID("tangent", 0x3c52b7db8f51de22)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.tangents,        StringID("tangent", 0x3c52b7db8f51de22)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, StringID("biTangents", 0xc9ccb62e082e82a2)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.biTangents,   StringID("biTangents", 0xc9ccb62e082e82a2)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC2, StringID("texCoord", 0xa0a9c94137957633)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec2>(data.uvs[0],         StringID("texCoord", 0xa0a9c94137957633)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC4, StringID("blendWeight", 0x4e02a6961ed59218)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.weights,     StringID("blendWeight", 0x4e02a6961ed59218)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC4, StringID("blendIndice", 0x7beee96843342fb4)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.boneIndices, StringID("blendIndice", 0x7beee96843342fb4)); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC4, StringID("color", 0x77f5c18e246c6638)), LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.colors,            StringID("color", 0x77f5c18e246c6638)); })
};

namespace AGE
{
	struct LoadingCallback
	{
		LoadingCallback(std::size_t target, std::function<void(void)> callback)
		{
			ptr = std::make_shared<Internal>(target, callback);
		}
		LoadingCallback(const LoadingCallback &o)
		{
			ptr = o.ptr;
		}
		~LoadingCallback()
		{
		}
		void increment()
		{
			if (++(ptr->counter) == ptr->target)
			{
				ptr->callback();
			}
		}
	private:
		struct Internal
		{
			const size_t target;
			std::atomic_size_t counter;
			std::function<void(void)> callback;

			Internal(std::size_t _target, std::function<void(void)> _callback)
				: target(_target), counter(0), callback(_callback)
			{}
			Internal() = delete;
		};
		std::shared_ptr<Internal> ptr = nullptr;

		LoadingCallback() = delete;
		LoadingCallback& operator=(const LoadingCallback &) = delete;
	};


	AssetsManager::AssetsManager()
	{
		QueueOwner::registerSharedCallback<LoadAssetMessage>([](LoadAssetMessage &msg){msg.setValue(msg.function()); });
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
			return _materials[filePath.getFullName()].second;
		}
		return nullptr;
	}
	
	bool AssetsManager::material_was_reloaded(const OldFile &_filePath) const
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		//get the material adaptered if not return false
		auto &it_material = _materials.find(filePath.getFullName());
		if (it_material == _materials.end()) {
			return false;
		}
		if (*it_material->second.first == true) {
			*it_material->second.first = false;
			return true;
		}
		return false;
	}

	bool AssetsManager::loadMaterial(const OldFile &_filePath, const StringID &loadingChannel)
	{
		auto material = std::make_shared<MaterialSetInstance>();
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_materials.find(filePath.getFullName()) != std::end(_materials))
			{
				return (true);
			}
			_materials.insert(std::make_pair(filePath.getFullName(), std::make_pair(std::make_shared<bool>(true), material)));
		}
		auto future = TMQ::TaskManager::emplaceSharedFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			SCOPE_profile_cpu_i("AssetsLoad", "LoadMaterial");
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

				materialSubset.shininess = material_data.shininess;

				materialSubset.diffuse = material_data.diffuse;

				materialSubset.ambient = material_data.ambient;

				materialSubset.emissive = material_data.emissive;

				materialSubset.reflective = material_data.reflective;

				materialSubset.specular = material_data.specular;

				materialSubset.normalTex = std::static_pointer_cast<Texture2D>(loadTexture(material_data.normalTexPath, loadingChannel));

				auto diffuseTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.diffuseTexPath, loadingChannel));
				if (diffuseTexPtr == nullptr)
				{
					diffuseTexPtr = std::static_pointer_cast<Texture2D>(loadTexture("ambiant_default.dds", loadingChannel));
				}
				materialSubset.diffuseTex = diffuseTexPtr;

				auto ambientTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.ambientTexPath, loadingChannel));
				materialSubset.ambientTex = ambientTexPtr;

				auto emissiveTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.emissiveTexPath, loadingChannel));
				materialSubset.emissiveTex = emissiveTexPtr;

				auto reflectiveTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.reflectiveTexPath, loadingChannel));
				materialSubset.reflectiveTex = reflectiveTexPtr;

				auto specularTexPtr = std::static_pointer_cast<Texture2D>(loadTexture(material_data.specularTexPath, loadingChannel));
				materialSubset.specularTex = specularTexPtr;

				materialSubset.scaleUVs = material_data.scaleUVs;
			}
			material->_valid = true;
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	std::shared_ptr<ITexture> AssetsManager::loadTexture(
		const OldFile &_filePath
		, const StringID &loadingChannel)
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

		auto future = TMQ::TaskManager::emplaceRenderFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			SCOPE_profile_cpu_i("AssetsLoad", "LoadTexture");

			std::shared_ptr<ATexture> loaded = OpenGLDDSLoader::loadDDSFile(filePath);
			if (loaded == nullptr)
				return AssetsLoadingResult(true, "Could not load the texture.\n");
			if (loaded->type() != GL_TEXTURE_2D)
				return AssetsLoadingResult(true, "Texture is not of the right type.\n");
			*texture = *std::static_pointer_cast<Texture2D>(loaded);
			return AssetsLoadingResult(true);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return texture;
	}

	std::shared_ptr<TextureCubeMap> AssetsManager::loadCubeMap(std::string const &name, OldFile &_filePath, const StringID &loadingChannel)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());

		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_cubeMaps.find(name) != std::end(_cubeMaps))
			{
				return _cubeMaps[name];
			}
		}

		auto texture = std::make_shared<TextureCubeMap>();
		{
			std::lock_guard<std::mutex> lock(_mutex);
			_cubeMaps.insert(std::make_pair(name, texture));
		}

		auto future = TMQ::TaskManager::emplaceRenderFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			std::shared_ptr<ATexture> loaded = OpenGLDDSLoader::loadDDSFile(filePath);
			if (loaded == nullptr)
				return AssetsLoadingResult(true, "Could not load the texture.\n");
			if (loaded->type() != GL_TEXTURE_CUBE_MAP)
				return AssetsLoadingResult(true, "Texture is not of the right type.\n");
			*texture = *std::static_pointer_cast<TextureCubeMap>(loaded);
			return AssetsLoadingResult(true);
		});
		pushNewAsset(loadingChannel, filePath.getFullName(), future);
		return texture;
	}

	bool AssetsManager::loadAnimation(const OldFile &_filePath, const StringID &loadingChannel)
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
			_animations.insert(std::make_pair(filePath.getFullName(), nullptr));
		}
		auto future = TMQ::TaskManager::emplaceSharedFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable {
			LoadingCallback callback(1, [=]()
			{
				std::lock_guard<std::mutex> lock(this->_mutex);
				this->_animations[filePath.getFullName()] = animation;
			});
			SCOPE_profile_cpu_i("AssetsLoad", "LoadAnimation");
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*animation.get());
			callback.increment();
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	std::shared_ptr<AnimationData> AssetsManager::getAnimation(const OldFile &_filePath)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		if (_animations.find(filePath.getFullName()) != std::end(_animations))
			return _animations[filePath.getFullName()];
		return nullptr;
	}

	bool AssetsManager::loadSkeleton(const OldFile &_filePath, const StringID &loadingChannel)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		auto skeleton = std::make_shared<Skeleton>(_filePath.getFileName().c_str());
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
			_skeletons.insert(std::make_pair(filePath.getFullName(), nullptr));
		}

		LoadingCallback callback(1, [=]()
		{
			std::lock_guard<std::mutex> lock(this->_mutex);
			this->_skeletons[filePath.getFullName()] = skeleton;
		});

		auto future = TMQ::TaskManager::emplaceSharedFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable {
			SCOPE_profile_cpu_i("AssetsLoad", "LoadSkeleton");
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			ar(*skeleton.get());
			callback.increment();
			return true;
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}


	std::shared_ptr<Skeleton> AssetsManager::getSkeleton(const OldFile &_filePath)
	{
		std::lock_guard<std::mutex> lock(_mutex);

		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		if (_skeletons.find(filePath.getFullName()) != std::end(_skeletons))
			return _skeletons[filePath.getFullName()];
		return nullptr;
	}

	bool AssetsManager::loadMesh(const OldFile &_filePath, const StringID &loadingChannel)
	{
		auto meshInstance = std::make_shared<MeshInstance>();
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_meshs.find(filePath.getFullName()) != std::end(_meshs))
			{
				return (true);
			}
			this->_meshs.insert(std::make_pair(filePath.getFullName(), nullptr));
		}
		auto future = TMQ::TaskManager::emplaceSharedFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
		{
			SCOPE_profile_cpu_i("AssetsLoad", "LoadMesh");

			if (!filePath.exists())
			{
				return AssetsLoadingResult(true, std::string("AssetsManager : Mesh File [" + filePath.getFullName() + "] does not exists.\n"));
			}
			std::ifstream ifs(filePath.getFullName(), std::ios::binary);
			cereal::PortableBinaryInputArchive ar(ifs);
			std::shared_ptr<MeshData> data = std::make_shared<MeshData>();
			ar(*data.get());
			meshInstance->meshData = data;
			meshInstance->subMeshs.resize(data->subMeshs.size());
			meshInstance->name = data->name;
			meshInstance->path = _filePath.getFullName();


			LoadingCallback callback(data->subMeshs.size(), [=]()
			{
				std::lock_guard<std::mutex> lock(this->_mutex);
				this->_meshs[filePath.getFullName()] = meshInstance;
			});

			// If no vertex pool correspond to submesh
			for (std::size_t i = 0; i < data->subMeshs.size(); ++i)
			{
				auto future = TMQ::TaskManager::emplaceSharedFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable
				{
					loadSubmesh(data, i, &meshInstance->subMeshs[i], loadingChannel, callback);
					return AssetsLoadingResult(false);
				});
				pushNewAsset(loadingChannel, data->subMeshs[i].name, future);
			}
			meshInstance->_valid = true;
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return (true);
	}

	void AssetsManager::loadSubmesh(std::shared_ptr<MeshData> fileData, std::size_t index, SubMeshInstance *mesh, const StringID &loadingChannel, LoadingCallback callback)
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
		auto future = TMQ::TaskManager::emplaceRenderFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable {
			SCOPE_profile_cpu_i("AssetsLoad", "LoadSubMesh");

			auto &paintingManager = GetRenderThread()->paintingManager;
			std::vector<std::pair<GLenum, StringID>> types;
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
			mesh->isSkinned = data.infos.test(MeshInfos::BoneIndices);
			for (auto i = 0ull; i < data.infos.size(); ++i)
			{
				if (data.infos.test(i))
				{
					g_InfosTypes[i].second(*vertices, i, data);
				}
			}
			vertices->set_indices(data.indices);
			callback.increment();
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, data.name, future);
	}

	void AssetsManager::pushNewAsset(const StringID &loadingChannel, const std::string &filename, std::future<AssetsManager::AssetsLoadingResult> &future)
	{
		std::shared_ptr<AssetsManager::AssetsLoadingChannel> channel = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_loadingChannels.find(loadingChannel) == std::end(_loadingChannels))
			{
				_loadingChannels.insert(std::make_pair(loadingChannel, std::make_shared<AssetsManager::AssetsLoadingChannel>()));
			}
			channel = _loadingChannels[loadingChannel];
			channel->pushNewAsset(filename, future);
		}
	}

	void AssetsManager::pushNewCallback(const StringID &loadingChannel, AScene *currentScene, std::function<void()> &callback)
	{
		std::shared_ptr<AssetsManager::AssetsLoadingChannel> channel = nullptr;
		{
			std::lock_guard<std::mutex> lock(_mutex);
			if (_loadingChannels.find(loadingChannel) == std::end(_loadingChannels))
			{
				_loadingChannels.insert(std::make_pair(loadingChannel, std::make_shared<AssetsManager::AssetsLoadingChannel>()));
			}
			channel = _loadingChannels[loadingChannel];
			channel->pushNewCallback(callback, currentScene);
		}
	}

	bool AssetsManager::AssetsLoadingChannel::updateList(int &noLoaded, int &total)
	{
		std::lock_guard<std::mutex> lock(_mutex);
		std::size_t i = 0;
		_list.remove_if([&](AssetsManager::AssetsLoadingStatus &e){
			//if (i > 30)
			//	return false;
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
		std::lock_guard<std::mutex> lock(_mutex);

		for (auto &e : _callbacks)
		{
			if (e.callback && e.scene != nullptr)
			{
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
		SCOPE_profile_cpu_function("Main thread");

		std::vector<StringID> toErase;
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
#ifdef AGE_ENABLE_IMGUI
			ImGui::Begin("ASSETS LOADING", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
			ImGui::SetWindowPos(ImVec2(30, 30));
			ImGui::Text("Assets loading : %i / %i", toLoad, total);
			ImGui::End();
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

	void AssetsManager::loadPackage(const OldFile &packagePath, const StringID &loadingChannel)
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

	void AssetsManager::loadPackage(const AssetsPackage &package, const StringID &loadingChannel)
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

	void AssetsManager::savePackage(const AssetsPackage &package, const std::string &filePath)
	{
		std::ofstream file(filePath, std::ios::binary);
		AGE_ASSERT(file.is_open());
		{
			auto ar = cereal::JSONOutputArchive(file);
			ar(package);
		}
	}

	std::shared_ptr<Texture2D> const &AssetsManager::getPointLightTexture()
	{
		if (!_pointLight)
			_pointLight = std::static_pointer_cast<Texture2D>(loadTexture("pointlight.dds", "BASIC_ASSETS"));
		return _pointLight;
	}

	std::shared_ptr<Texture2D> const &AssetsManager::getSpotLightTexture()
	{
		if (!_spotLight)
			_spotLight = std::static_pointer_cast<Texture2D>(loadTexture("spotlight.dds", "BASIC_ASSETS"));
		return _spotLight;
	}

	bool AssetsManager::BitsetComparer::operator() (const std::bitset<MeshInfos::END> &b1, const std::bitset<MeshInfos::END> &b2) const
	{
		return b1.to_ulong() < b2.to_ulong();
	}

	AssetsManager::AssetsLoadingResult::AssetsLoadingResult(bool _error, const std::string &_errorMessage)
		: error(_error)
		, errorMessage(_errorMessage)
	{}

	AssetsManager::AssetsLoadingStatus::AssetsLoadingStatus(AssetsLoadingStatus &&o)
		: filename(std::move(o.filename))
		, future(std::move(o.future))
		, result(std::move(o.result))
	{
	}

	AssetsManager::AssetsLoadingStatus::AssetsLoadingStatus(const std::string &_fileName, std::future<AssetsLoadingResult> &_future)
		: filename(_fileName)
	{
		future = std::move(_future);
	}

	AssetsManager::LoadAssetMessage::LoadAssetMessage(const std::function<AssetsLoadingResult()> &_function)
		: function(_function)
	{}

}
