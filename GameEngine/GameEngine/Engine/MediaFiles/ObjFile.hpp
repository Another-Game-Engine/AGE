#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <MediaFiles/MaterialFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>
#include <OpenGL/VertexBuffer.hh>

struct ObjFile : public MediaFile<ObjFile>
{
	ObjFile() : MediaFile<ObjFile>()
	{
		_type = OBJ;
	}
	virtual ~ObjFile()
	{}

	ObjFile(const ObjFile &o)
		: MediaFile<ObjFile>(o)
	{
		geometries = o.geometries;
		material = o.material;
	}

	ObjFile &operator=(const ObjFile &o)
	{
		if (&o != this)
		{
			geometries = o.geometries;
			material = o.material;
		}
		return *this;
	}

	struct Geometry
	{
		std::string                 name;
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec4>		normals;	// normals
		std::vector<glm::vec4>		colors;		// vertices colors
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<unsigned int>	indices;	// indices
		OpenGLTools::VertexBuffer	buffer;

		Geometry()
			: name("")
		{}

		~Geometry()
		{}

		Geometry(const Geometry &o)
			: name("")
		{
			name = o.name;
			vertices = o.vertices;
			normals = o.normals;
			colors = o.colors;
			uvs = o.uvs;
			indices = o.indices;
		}

		Geometry &operator=(const Geometry &o)
		{
			if (&o != this)
			{
				name = o.name;
				vertices = o.vertices;
				normals = o.normals;
				colors = o.colors;
				uvs = o.uvs;
				indices = o.indices;
			}
			return *this;
		}

		template <typename Archive>
		void save(Archive &ar) const
		{
			ar(name,vertices, normals,colors,uvs, indices);
		}

		template <typename Archive>
		void load(Archive &ar)
		{
			ar(name,vertices, normals,colors,uvs, indices);
			buffer.init(vertices.size(), &indices[0]);
			buffer.addAttribute(OpenGLTools::Attribute(sizeof(float)* 4, 4, GL_FLOAT));
			buffer.addAttribute(OpenGLTools::Attribute(sizeof(float)* 4, 4, GL_FLOAT));
			buffer.addAttribute(OpenGLTools::Attribute(sizeof(float)* 4, 4, GL_FLOAT));
			buffer.addAttribute(OpenGLTools::Attribute(sizeof(float)* 2, 2, GL_FLOAT));

			buffer.setBuffer(0, reinterpret_cast<byte *>(&vertices[0].x));
			if (colors.size())
				buffer.setBuffer(1, reinterpret_cast<byte *>(&colors[0].x));
			if (normals.size())
				buffer.setBuffer(2, reinterpret_cast<byte *>(&normals[0].x));
			if (uvs.size())
				buffer.setBuffer(3, reinterpret_cast<byte *>(&uvs[0].x));
		}
	};

	std::vector<Geometry> geometries;
	std::shared_ptr<MaterialFile> material;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		res->manager = manager;
		ar(static_cast<ObjFile&>(*res));
		return res;
	}

	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(CEREAL_NVP(geometries));
		ar(cereal::make_nvp("material", material->path.getFullName()));
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		ar(geometries);
		std::string matName;
		ar(matName);
		if (matName != "NULL")
			material = std::static_pointer_cast<MaterialFile>(AMediaFile::loadFromFile<Archive>(File(matName)));
	}

};

#endif   //__OBJ_FILE_HPP__