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
#include <Render/ShadingManager.hh>

ObjFile::ObjFile() : MediaFile<ObjFile>()
{
	_type = OBJ;
}

ObjFile::~ObjFile()
{
}

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
		, geomanager(nullptr)
{
		if (geomanager)
		{
			geomanager->rmIndices(glindices);
			geomanager->rmVertices(glvertices);
		}
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
		_dependencyManager = o._dependencyManager;
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
			_dependencyManager = o._dependencyManager;
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
		geomanager = &_dependencyManager.lock()->getInstance<gl::ShadingManager>()->geometryManager;
		void *buffer[4] = { &vertices[0].x ,
							&colors[0].x,
							&normals[0].x,
							&uvs[0].x };
		size_t nbrBuffer[4] = {vertices.size() * 4 * sizeof(float),
								colors.size() * 4 * sizeof(float),
								normals.size() * 4 * sizeof(float),
								uvs.size() * 2 * sizeof(float)};
		glvertices = geomanager->addVertices(vertices.size(), 4, nbrBuffer, buffer);
		glindices = geomanager->addIndices(indices.size(), &indices[0]);
		geomanager->attachVerticesToVertexPool(glvertices, geomanager->getVertexPool(0));
		geomanager->attachIndicesToIndexPool(glindices, geomanager->getIndexPool(0));
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
		e._dependencyManager = _dependencyManager;
		e.init();
	}
	std::string matName;
	ar(matName);
	auto manager = _dependencyManager.lock()->getInstance<gl::ShadingManager>();
	if (matName != "NULL")
		material = std::static_pointer_cast<MaterialFile>(_dependencyManager.lock()->getInstance<AssetsManager>()->loadFromFile(File(matName), *manager));
}