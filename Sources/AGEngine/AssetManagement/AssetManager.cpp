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
#include <Render/Properties/Transformation.hh>
#include <Render/Properties/Materials/Color.hh>
#include <Render/Properties/Materials/MapColor.hh>
#include <Render/Properties/Materials/ScaleUVs.hpp>
#include <Render/Properties/Materials/Ratio.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>

#include <AssetManagement/OpenGLDDSLoader.hh>

#include <Utils/Profiler.hpp>

#include <Configuration.hpp>

#ifdef AGE_ENABLE_IMGUI
#include <imgui/imgui.h>
#endif

# define LAMBDA_FUNCTION [](AGE::Vertices &vertices, size_t index, AGE::SubMeshData const &data)

static std::pair<std::pair<GLenum, std::string>, std::function<void(AGE::Vertices &vertices, size_t index, AGE::SubMeshData const &data)>> g_InfosTypes[AGE::MeshInfos::END] =
{
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, std::string("position")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.positions, std::string("position")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, std::string("normal")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.normals, std::string("normal")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, std::string("tangent")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.tangents, std::string("tangent")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC3, std::string("biTangents")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec3>(data.biTangents, std::string("biTangents")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC2, std::string("texCoord")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec2>(data.uvs[0], std::string("texCoord")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC4, std::string("blendWeight")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.weights, std::string("blendWeight")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC4, std::string("blendIndice")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.boneIndices, std::string("blendIndice")); }),
	std::make_pair(std::make_pair(GL_FLOAT_VEC4, std::string("color")), LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.colors, std::string("color")); })
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

	bool AssetsManager::reloadMaterial(const OldFile &_filePath, const std::string &loadingChannel)
	{
		OldFile filePath(_assetsDirectory + _filePath.getFullName());
		//get the material adaptered if not return false
		auto it_material = _materials.find(filePath.getFullName());
		if (it_material == _materials.end()) {
			return false;
		}
		auto &material = it_material->second;
		material.second->datas.clear();
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
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
			material.second->name = material_data_set->name;
			material.second->path = _filePath.getFullName();
			for (auto &material_data : material_data_set->collection)
			{

				material.second->datas.push_back(MaterialInstance());
				auto &materialSubset = material.second->datas.back();

				auto &shininess = std::make_shared<Ratio>("shininess");
				materialSubset._properties.push_back(shininess);
				shininess->set(material_data.shininess);

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
				if (diffuseTexPtr == nullptr)
				{
					diffuseTexPtr = std::static_pointer_cast<Texture2D>(loadTexture("ambiant_default.dds", loadingChannel));
				}
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

				auto scaleUVs = std::make_shared<ScaleUVs>();
				materialSubset._properties.push_back(scaleUVs);
				scaleUVs->set(material_data.scaleUVs);
			}
			material.second->_valid = true;
			*material.first = true;
			return AssetsLoadingResult(false);
		});
		pushNewAsset(loadingChannel, _filePath.getFullName(), future);
		return true;
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
			_materials.insert(std::make_pair(filePath.getFullName(), std::make_pair(std::make_shared<bool>(true), material)));
		}
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
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

				auto &shininess = std::make_shared<Ratio>("shininess");
				materialSubset._properties.push_back(shininess);
				shininess->set(material_data.shininess);

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
				if (diffuseTexPtr == nullptr)
				{
					diffuseTexPtr = std::static_pointer_cast<Texture2D>(loadTexture("ambiant_default.dds", loadingChannel));
				}
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

				auto scaleUVs = std::make_shared<ScaleUVs>();
				materialSubset._properties.push_back(scaleUVs);
				scaleUVs->set(material_data.scaleUVs);

//				auto futureSubMaterial = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<Tasks::Render::AddMaterial, MaterialInstance>(material_data);
//				auto subMaterial = futureSubMaterial.get();
//				material_set->datas.emplace_back(subMaterial);
			}
			material->_valid = true;
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

	std::shared_ptr<TextureCubeMap> AssetsManager::loadCubeMap(std::string const &name, OldFile &_filePath, const std::string &loadingChannel)
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

		auto future = AGE::GetRenderThread()->getQueue()->emplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
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
			_animations.insert(std::make_pair(filePath.getFullName(), nullptr));
		}
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable {
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

	bool AssetsManager::loadSkeleton(const OldFile &_filePath, const std::string &loadingChannel)
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

		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable {
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
			this->_meshs.insert(std::make_pair(filePath.getFullName(), nullptr));
		}
		auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]()
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
				auto future = AGE::EmplaceFutureTask<LoadAssetMessage, AssetsLoadingResult>([=]() mutable
				{
					loadSubmesh(data, i, &meshInstance->subMeshs[i], loadingChannel);
					callback.increment();
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
			SCOPE_profile_cpu_i("AssetsLoad", "LoadSubMesh");

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
			mesh->isSkinned = data.infos.test(MeshInfos::BoneIndices);
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
			channel->pushNewAsset(filename, future);
		}
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
#ifdef AGE_ENABLE_IMGUI
			ImGui::Begin("ASSETS LOADING", (bool*)1, ImVec2(0, 0), 0.3f, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoSavedSettings);
			ImGui::SetWindowPos(ImVec2(30, 30));
			ImGui::Text("Assets loading : %s / %s", std::to_string(toLoad).c_str(), std::to_string(total).c_str());
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

	std::shared_ptr<Texture2D> const &AssetsManager::getPointLightTexture()
	{
		if (!_pointLight)
			_pointLight = std::static_pointer_cast<Texture2D>(loadTexture("pointlight.dds", ""));
		return _pointLight;
	}

	std::shared_ptr<Texture2D> const &AssetsManager::getSpotLightTexture()
	{
		if (!_spotLight)
			_spotLight = std::static_pointer_cast<Texture2D>(loadTexture("spotlight.dds", ""));
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