#pragma once

#include <memory>
#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/File.hpp>
#include <Geometry/Mesh.hpp>

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
		std::shared_ptr<Animation> loadAnimation(const File &filePath);
		std::shared_ptr<Skeleton> loadSkeleton(const File &filePath);
		std::shared_ptr<MeshInstance> loadMesh(const File &filePath);

	private:
		// Vertices pools used by meshs
		std::map<std::bitset<MeshInfos::END>, std::pair<gl::Key<gl::VertexPool> , gl::Key<gl::IndexPool>>, BitsetComparer> _pools;
		// Mesh collection
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;
		//Skeleton collection
		std::map<std::string, std::shared_ptr<Skeleton>> _skeletons;
		//Animation collection
		std::map<std::string, std::shared_ptr<Animation>> _animations;

		void loadSubmesh(SubMeshData &data, SubMeshInstance &mesh);
		// Create pool for meshs
		void createPool(const std::bitset<MeshInfos::END> &infos);
	};
}