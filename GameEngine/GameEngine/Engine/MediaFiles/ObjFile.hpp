#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <OpenGL/Vertices.hh>
#include <OpenGL/VerticesManager.hh>
#include <Utils/DependenciesInjector.hpp>

struct ObjFile : public MediaFile<ObjFile>
{
	ObjFile();
	virtual ~ObjFile();
	ObjFile(const ObjFile &o);
	ObjFile &operator=(const ObjFile &o);
	struct Geometry
	{
		std::string                 name;
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec4>		normals;	// normals
		std::vector<glm::vec4>		colors;		// vertices colors
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<unsigned int>	indices;	// indices
#if !TEST_ARCHI
		Vertice<4>					buffer; //-V112
#endif
	private:
		std::weak_ptr<DependenciesInjector> _dpyManager;
		friend struct ObjFile;
	public:
		Geometry();
		~Geometry();
		Geometry(const Geometry &o);
		Geometry &operator=(const Geometry &o);
		void save(cereal::PortableBinaryOutputArchive &ar) const;
		void load(cereal::PortableBinaryInputArchive &ar);
		void init();
	};

	std::vector<Geometry> geometries;
	std::shared_ptr<MaterialFile> material;

	void save(cereal::PortableBinaryOutputArchive &ar) const;
	void load(cereal::PortableBinaryInputArchive &ar);
};

#endif   //__OBJ_FILE_HPP__
