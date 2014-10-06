#pragma once

#include <memory>
#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/File.hpp>
#include <Render/GeometryManager.hh>
#include <bitset>
#include <Geometry/Mesh.hpp>
#include <Render/Storage.hh>

namespace AGE
{
	struct Skeleton;
	struct Animation;
	struct MaterialSetInstance;

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
		std::shared_ptr<Animation> loadAnimation(const File &filePath);
		std::shared_ptr<Animation> getAnimation(const File &filePath);
		std::shared_ptr<Skeleton> loadSkeleton(const File &filePath);
		std::shared_ptr<Skeleton> getSkeleton(const File &filePath);
		std::shared_ptr<MeshInstance> loadMesh(const File &filePath, const std::vector<MeshInfos> &loadOrder = std::vector<MeshInfos>());
		std::shared_ptr<MeshInstance> getMesh(const File &filePath);
		std::shared_ptr<MaterialSetInstance> loadMaterial(const File &filePath);
		std::shared_ptr<MaterialSetInstance> getMaterial(const File &filePath);
		gl::Key<gl::Texture> loadTexture(const File &filepath);
		void setAssetsDirectory(const std::string &path) { _assetsDirectory = path; }
	private:
		std::string _assetsDirectory;
		// Vertices pools used by meshs
		std::map<std::bitset<MeshInfos::END>, std::pair<gl::Key<gl::VertexPool> , gl::Key<gl::IndexPool>>, BitsetComparer> _pools;
		// Mesh collection
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;
		//Skeleton collection
		std::map<std::string, std::shared_ptr<Skeleton>> _skeletons;
		//Animation collection
		std::map<std::string, std::shared_ptr<Animation>> _animations;
		//Material collection
		std::map<std::string, std::shared_ptr<MaterialSetInstance>> _materials;
		//Texture collection
		std::map<std::string, gl::Key<gl::Texture>> _textures;

		void loadSubmesh(SubMeshData &data, SubMeshInstance &mesh, const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos);
		// Create pool for meshs
		void createPool(const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos);
	};
}