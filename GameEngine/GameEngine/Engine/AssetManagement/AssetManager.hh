#pragma once

#include <memory>
#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/OldFile.hpp>
#include <bitset>
#include <AssetManagement/Data/MeshData.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <map>
#include <future>
#include <TMQ/message.hpp>
#include <functional>
#include <string>

#include <cereal/archives/json.hpp>
#include <cereal/types/unordered_set.hpp>
#include <Render/Textures/Texture3D.hh>

namespace AGE
{

	struct MaterialSetInstance;
	struct MeshInstance;
	struct SubMeshInstance;
	class ITexture;
	class Painter;

# define LAMBDA_FUNCTION [](Vertices &vertices, size_t index, SubMeshData const &data)

static std::pair<std::pair<GLenum, std::string>, std::function<void(Vertices &vertices, size_t index, SubMeshData const &data)>> g_InfosTypes[MeshInfos::END] =
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

	struct Skeleton;
	struct AnimationData;
	class AScene;

	class AssetsManager : public Dependency<AssetsManager>
	{
	public:
		// collection of assets
		struct AssetsPackage
		{
			std::unordered_set<std::string> meshs;
			std::unordered_set<std::string> materials;

			template <typename Archive>
			void serialize(Archive &ar, const std::uint32_t version)
			{
				ar(meshs, materials);
			}
		};
	private:
		struct BitsetComparer {
			bool operator() (const std::bitset<MeshInfos::END> &b1, const std::bitset<MeshInfos::END> &b2) const
			{
				return b1.to_ulong() < b2.to_ulong();
			}
		};

	public:
		AssetsManager();
		struct AssetsLoadingResult
		{
			bool error = false;
			std::string errorMessage = "";
			AssetsLoadingResult() = default;
			AssetsLoadingResult(bool _error, const std::string &_errorMessage = "")
				: error(_error)
				, errorMessage(_errorMessage)
			{}
		};

	public:
		struct AssetsLoadingStatus
		{
			std::string filename = "";
			std::future < AssetsLoadingResult > future;
			AssetsLoadingResult result;
			AssetsLoadingStatus(const AssetsLoadingStatus&) = delete;
			AssetsLoadingStatus(AssetsLoadingStatus &&o)
				: filename(std::move(o.filename))
				, future(std::move(o.future))
				, result(std::move(o.result))
			{

			}
			AssetsLoadingStatus(const std::string &_fileName, std::future<AssetsLoadingResult> &_future)
				: filename(_fileName)
			{
				future = std::move(_future);
			}
		};

	public:
		class AssetsLoadingChannel
		{
			struct CallBackContainer
			{
				AScene *scene;
				std::function<void()> callback;
			};

			std::list<AssetsLoadingStatus> _list;
			std::string _errorMessages = "";
			std::size_t _maxAssets = 0;
			std::vector<CallBackContainer> _callbacks;
		public:
			// return false if error
			bool updateList(int &noLoaded, int &total);
			inline const std::string &getErrorMessages() const { return _errorMessages; }
			void callCallbacks();
		private:
			std::mutex _mutex;
			void pushNewAsset(const std::string &filename, std::future<AssetsLoadingResult> &future);
			void pushNewCallback(std::function<void()> &callback, AScene *currentScene);
			friend class AssetsManager;
		};

	public:
		struct LoadAssetMessage : public TMQ::FutureData < AssetsLoadingResult >
		{
			std::function<AssetsLoadingResult()> function;
			LoadAssetMessage(const std::function<AssetsLoadingResult()> &_function)
				: function(_function)
			{}
		};

	public:
		void loadPackage(const OldFile &packagePath, const std::string &loadingChannel = "");
		void loadPackage(const AssetsPackage &package, const std::string &loadingChannel = "");
		void savePackage(const AssetsPackage &package, const std::string filePath);
		bool loadAnimation(const OldFile &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<AnimationData> getAnimation(const OldFile &filePath);
		bool loadSkeleton(const OldFile &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<Skeleton> getSkeleton(const OldFile &filePath);
		bool loadMaterial(const OldFile &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<MaterialSetInstance> getMaterial(const OldFile &filePath);
		std::shared_ptr<MeshInstance> getMesh(const OldFile &filePath);
		std::shared_ptr<ITexture> loadTexture(const OldFile &filepath, const std::string &loadingChannel);
		std::shared_ptr<Texture3D> loadSkybox(std::string const &name, OldFile &_filePath, std::array<std::string, 6> const &textures, const std::string &loadingChannel);
		bool loadMesh(const OldFile &filePath, const std::string &loadingChannel = "");
		void setAssetsDirectory(const std::string &path) { _assetsDirectory = path; }
		void update();
		bool isLoading();
		void pushNewCallback(const std::string &loadingChannel, AScene *currentScene, std::function<void()> &callback);
		std::shared_ptr<ITexture> const &getPointLightTexture();
		std::shared_ptr<ITexture> const &getSpotLightTexture();

private:
		std::string _assetsDirectory;
		std::map<std::bitset<MeshInfos::END>, Key<Painter>, BitsetComparer> _painters;
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;
		std::map<std::string, std::shared_ptr<Skeleton>> _skeletons;
		std::map<std::string, std::shared_ptr<AnimationData>> _animations;
		std::map<std::string, std::shared_ptr<MaterialSetInstance>> _materials;
		std::map<std::string, std::shared_ptr<ITexture>> _textures;
		std::map<std::string, std::shared_ptr<Texture3D>> _skyboxes;
		std::map<std::string, std::shared_ptr<AssetsLoadingChannel>> _loadingChannels;
		std::shared_ptr<ITexture> _pointLight;
		std::shared_ptr<ITexture> _spotLight;
		std::mutex _mutex;
		std::atomic<bool> _isLoading;
	private:
		void pushNewAsset(const std::string &loadingChannel, const std::string &filename, std::future<AssetsLoadingResult> &future);
		void loadSubmesh(std::shared_ptr<MeshData> data, std::size_t index, SubMeshInstance *mesh, const std::string &loadingChannel);
	};
}

CEREAL_CLASS_VERSION(AGE::AssetsManager::AssetsPackage, 0)