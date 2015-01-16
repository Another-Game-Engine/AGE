#pragma once

#include <memory>
#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/File.hpp>
#include <bitset>
#include <Data/MeshData.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <map>
#include <future>
#include <TMQ/message.hpp>

struct MaterialSetInstance;
struct MeshInstance;
class ITexture;

namespace AGE
{
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

		struct LoadAssetMessage : public TMQ::FutureData < AssetsLoadingResult >
		{
			std::function<AssetsLoadingResult()> function;
			LoadAssetMessage(const std::function<AssetsLoadingResult()> &_function)
				: function(_function)
			{}
		};

		bool loadAnimation(const File &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<Animation> getAnimation(const File &filePath);
		bool loadSkeleton(const File &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<Skeleton> getSkeleton(const File &filePath);
		bool loadMaterial(const File &filePath, const std::string &loadingChannel = "");
		std::shared_ptr<MaterialSetInstance> getMaterial(const File &filePath);
		std::shared_ptr<MeshInstance> loadMesh(const File &filePath);
		std::shared_ptr<MeshInstance> getMesh(const File &filePath);
		bool loadTexture(const File &filepath, const std::string &loadingChannel = "");

//<<<<<<< HEAD

//		std::shared_ptr<MeshInstance> createMesh(std::string const &meshName, AGE::Vector<glm::vec4> const &positions, AGE::Vector<glm::vec4> const &colors, AGE::Vector<unsigned int> const &idx);
//		std::shared_ptr<Program> addProgram(std::string const &name, std::vector<std::shared_ptr<UnitProg>> const &u);
//		std::shared_ptr<Program> getProgram(std::string const &name) { assert(_programsMap.find(name) != _programsMap.end()); return (_programsMap[name]); }
//		std::shared_ptr<Painter> getPainter(std::bitset<MeshInfos::END> infos) { assert(_painters.find(infos) != _painters.end()); return (_painters[infos]); }
//=======
//		void loadMesh(const File &filePath, const std::vector<MeshInfos> &loadOrder = std::vector<MeshInfos>(), const std::string &loadingChannel = "");
//		std::shared_ptr<MeshInstance> getMesh(const File &filePath);
//		
//>>>>>>> master
		void setAssetsDirectory(const std::string &path) { _assetsDirectory = path; }
		void updateLoadingChannel(const std::string &channelName, std::size_t &total, std::size_t &to_load, std::string &error);

	private:
		std::string _assetsDirectory;
		std::vector<std::shared_ptr<Program>> _programs;
		std::map<std::string, std::shared_ptr<Program>> _programsMap;
		std::map<std::bitset<MeshInfos::END>, std::shared_ptr<Painter>, AssetsManager::BitsetComparer> _painters;
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;
		std::map<std::string, std::shared_ptr<Skeleton>> _skeletons;
		std::map<std::string, std::shared_ptr<Animation>> _animations;
		std::map<std::string, std::shared_ptr<MaterialSetInstance>> _materials;
		std::map<std::string, std::shared_ptr<ITexture>> _textures;
		std::map<std::string, std::shared_ptr<AssetsLoadingChannel>> _loadingChannels;
		std::mutex _mutex;

//<<<<<<< HEAD
//
//		void loadSubmesh(SubMeshData &data, SubMeshInstance *mesh, /*const std::vector<MeshInfos> &order,*/ const std::bitset<MeshInfos::END> &infos);
//=======
//		
//		
//		
//
//		void loadSubmesh(std::shared_ptr<MeshData> data, std::size_t index, SubMeshInstance *mesh, const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos, const std::string &loadingChannel);
//>>>>>>> master
		// Create pool for meshs
	private:
		void createPool(const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos);
		void pushNewAsset(const std::string &loadingChannel, const std::string &filename, std::future<AssetsLoadingResult> &future);
	};
}