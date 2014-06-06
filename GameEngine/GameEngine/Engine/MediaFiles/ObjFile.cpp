#include "ObjFile.hpp"
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <memory>
#include "AssetsManager.hpp"

ObjFile::ObjFile() : MediaFile<ObjFile>()
{
	_type = OBJ;
}

ObjFile::~ObjFile()
{}

ObjFile::ObjFile(const ObjFile &o)
: MediaFile<ObjFile>(o)
{
	geometries = o.geometries;
	material = o.material;
}

ObjFile &ObjFile::operator=(const ObjFile &o)
{
	if (&o != this)
	{
		geometries = o.geometries;
		material = o.material;
	}
	return *this;
}


ObjFile::Geometry::Geometry()
		: name("")
	{
	}

ObjFile::Geometry::~Geometry()
	{
	}

ObjFile::Geometry::Geometry(const ObjFile::Geometry &o)
		: name("")
	{
		name = o.name;
		vertices = o.vertices;
		normals = o.normals;
		colors = o.colors;
		uvs = o.uvs;
		indices = o.indices;
		_dpyManager = o._dpyManager;
		init();
	}

ObjFile::Geometry &ObjFile::Geometry::operator=(const ObjFile::Geometry &o)
	{
		if (&o != this)
		{
			name = o.name;
			vertices = o.vertices;
			normals = o.normals;
			colors = o.colors;
			uvs = o.uvs;
			indices = o.indices;
			_dpyManager = o._dpyManager;
			init();
		}
		return *this;
	}

void ObjFile::Geometry::save(cereal::PortableBinaryOutputArchive &ar) const
	{
		ar(name, vertices, normals, colors, uvs, indices);
	}

	void ObjFile::Geometry::load(cereal::PortableBinaryInputArchive &ar)
	{
		ar(name, vertices, normals, colors, uvs, indices);
	}

	void ObjFile::Geometry::init()
	{
		geomanager = _dpyManager.lock()->getInstance<gl::GeometryManager>();
		void *buffer[4] = { &vertices[0].x ,
							&colors[0].x,
							&normals[0].x,
							&uvs[0].x };
		size_t nbrBuffer[4] = {vertices.size() * 4 * sizeof(float),
								colors.size() * 4 * sizeof(float),
								normals.size() * 4 * sizeof(float),
								uvs.size() * 2 * sizeof(float)};
		glvertices = geomanager->addVertices(vertices.size(), 4, nbrBuffer, buffer);
		buffer[0] = &indices[0];
		nbrBuffer[0] = indices.size() * sizeof(unsigned int);
		glindices = geomanager->addVertices(indices.size(), 1, nbrBuffer, buffer);
		geomanager->attachVerticesToVertexPool(glvertices, geomanager->getVertexPool(0));
		geomanager->attachVerticesToIndexPool(glindices, geomanager->getIndexPool(0));
		geomanager->attachIndexPoolToVertexPool(geomanager->getVertexPool(0), geomanager->getIndexPool(0));
	}

void ObjFile::save(cereal::PortableBinaryOutputArchive &ar) const
{
	ar(CEREAL_NVP(geometries));
	ar(cereal::make_nvp("material", material->path.getFullName()));
}

void ObjFile::load(cereal::PortableBinaryInputArchive &ar)
{
	ar(geometries);
	for (auto &e : geometries)
	{
		e._dpyManager = _dpyManager;
		e.init();
	}
	std::string matName;
	ar(matName);
	if (matName != "NULL")
		material = std::static_pointer_cast<MaterialFile>(_dpyManager.lock()->getInstance<AssetsManager>()->loadFromFile(File(matName)));
}