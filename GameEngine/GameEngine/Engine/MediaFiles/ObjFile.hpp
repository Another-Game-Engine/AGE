#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>
#include <OpenGL/Data.hh>
#include <Utils/DependenciesInjector.hpp>

namespace gl { class GeometryManager; }

struct ObjFile : public MediaFile<ObjFile>
{
	ObjFile();
	virtual ~ObjFile();
	ObjFile(const ObjFile &o);
	ObjFile &operator=(const ObjFile &o);
	struct Geometry
	{
		std::string                 name;
		AGE::Vector<glm::vec4>		vertices;	// vertices positions
		AGE::Vector<glm::vec4>		normals;	// normals
		AGE::Vector<glm::vec4>		colors;		// vertices colors
		AGE::Vector<glm::vec2>		uvs;		// texture coordinates
		AGE::Vector<unsigned int>	indices;	// indices
		gl::Key<gl::Vertices>		glvertices;	// vertices
		gl::Key<gl::Indices>		glindices;	// indices
		gl::GeometryManager *geomanager; //manager
		//Vertice<4>					buffer; //-V112
	private:
		std::weak_ptr<DependenciesInjector> _dependencyManager;
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

	AGE::Vector<Geometry> geometries;
	std::shared_ptr<MaterialFile> material;

	void save(cereal::PortableBinaryOutputArchive &ar) const;
	void load(cereal::PortableBinaryInputArchive &ar);
};

#endif   //__OBJ_FILE_HPP__
