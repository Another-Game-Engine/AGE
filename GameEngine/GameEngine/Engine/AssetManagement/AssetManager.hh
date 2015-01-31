#pragma once

#include <memory>
#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/File.hpp>
#include <bitset>
#include <AssetManagement/Data/MeshData.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <Render/GeometryManagement/Data/Vertices.hh>
#include <map>
#include <future>
#include <TMQ/message.hpp>
#include <functional>

namespace AGE
{

	struct MaterialSetInstance;
	struct MeshInstance;
	struct SubMeshInstance;
	class ITexture;
	class Painter;

# define LAMBDA_FUNCTION [](Vertices &vertices, size_t index, SubMeshData const &data)

static std::pair<GLenum, std::function<void(Vertices &vertices, size_t index, SubMeshData const &data)>> g_InfosTypes[MeshInfos::END] =
{
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.positions, index); }),
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.normals, index); }),
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.tangents, index); }),
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.biTangents, index); }),
	std::make_pair(GL_FLOAT_VEC2, LAMBDA_FUNCTION{ vertices.set_data<glm::vec2>(data.uvs[0], index); }),
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.weights, index); }),
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.boneIndices, index); }),
	std::make_pair(GL_FLOAT_VEC4, LAMBDA_FUNCTION{ vertices.set_data<glm::vec4>(data.colors, index); })
};

	struct Skeleton;
	struct Animation;

	class AssetsManager : public Dependency<AssetsManager>
	{
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
				std::swap(future, _future);
			}
		};

	public:
		class AssetsLoadingChannel
		{
			std::list<AssetsLoadingStatus> _list;
			std::string _errorMessages = "";
			std::size_t _maxAssets = 0;
		public:
			// return false if error
			bool updateList(std::size_t &noLoaded, std::size_t &total);
			inline const std::string &getErrorMessages() const { return _errorMessages; }
		private:
			std::mutex _mutex;
			void pushNewAsset(const std::string &filename, std::future<AssetsLoadingResult> &future);
			friend class AssetsManager;
			std::chrono::system_clock::time_point _lastUpdate;
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
		bool loadAnimation(const File &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<Animation> getAnimation(const File &filePath);
		bool loadSkeleton(const File &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<Skeleton> getSkeleton(const File &filePath);
		bool loadMaterial(const File &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<MaterialSetInstance> getMaterial(const File &filePath);
		std::shared_ptr<MeshInstance> loadMesh(const File &filePath);
		std::shared_ptr<MeshInstance> getMesh(const File &filePath);
		bool loadTexture(const File &filepath, const std::string &loadingChannel, std::function<void(std::shared_ptr<ITexture> &texture)> &callback);
		bool loadMesh(const File &filePath, const std::vector<MeshInfos> &loadOrder, const std::string &loadingChannel = "");
		void setAssetsDirectory(const std::string &path) { _assetsDirectory = path; }
		void updateLoadingChannel(const std::string &channelName, std::size_t &total, std::size_t &to_load, std::string &error);

	private:
		std::string _assetsDirectory;
		std::map<std::bitset<MeshInfos::END>, Key<Painter>, BitsetComparer> _painters;
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;
		std::map<std::string, std::shared_ptr<Skeleton>> _skeletons;
		std::map<std::string, std::shared_ptr<Animation>> _animations;
		std::map<std::string, std::shared_ptr<MaterialSetInstance>> _materials;
		std::map<std::string, std::shared_ptr<ITexture>> _textures;
		std::map<std::string, std::shared_ptr<AssetsLoadingChannel>> _loadingChannels;
		std::mutex _mutex;

	private:
		void pushNewAsset(const std::string &loadingChannel, const std::string &filename, std::future<AssetsLoadingResult> &future);
		void loadSubmesh(std::shared_ptr<MeshData> data, std::size_t index, SubMeshInstance *mesh, const std::vector<MeshInfos> loadOrder, const std::string &loadingChannel);
	};
}