#pragma once

#include <memory>
#include <Utils/Dependency.hpp>
#include <Utils/DependenciesInjector.hpp>
#include <Utils/File.hpp>
#include <bitset>
#include <Geometry/Mesh.hpp>
#include <Render/Storage.hh>
#include <Render/Key.hh>
#include <Render/GeometryManagement/Painting/Painter.hh>
#include <map>

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
		std::shared_ptr<MeshInstance> loadMesh(const File &filePath);
		std::shared_ptr<MeshInstance> getMesh(const File &filePath);
		std::shared_ptr<MeshInstance> createMesh(AGE::Vector<glm::vec4> const &positions, AGE::Vector<glm::vec4> const &colors, AGE::Vector<unsigned int> const &idx);
		std::shared_ptr<Program> addProgram(std::string &&name, std::vector<std::shared_ptr<UnitProg>> const &u);
		void setAssetsDirectory(const std::string &path) { _assetsDirectory = path; }
	private:
		std::string _assetsDirectory;
		// Shaders
		std::vector<std::shared_ptr<Program>> _programs;
		std::map<std::string, std::shared_ptr<Program>> _programsMap;
		// Vertices pools used by meshs
		std::map<std::bitset<MeshInfos::END>, std::shared_ptr<Painter>, AssetsManager::BitsetComparer> _painters;
		// Mesh collection
		std::map<std::string, std::shared_ptr<MeshInstance>> _meshs;
		//Skeleton collection
		std::map<std::string, std::shared_ptr<Skeleton>> _skeletons;
		//Animation collection
		std::map<std::string, std::shared_ptr<Animation>> _animations;
		//Material collection
		//Texture collection

		void loadSubmesh(SubMeshData &data, SubMeshInstance *mesh, /*const std::vector<MeshInfos> &order,*/ const std::bitset<MeshInfos::END> &infos);
		// Create pool for meshs
		void createPool(const std::vector<MeshInfos> &order, const std::bitset<MeshInfos::END> &infos);
	};
}