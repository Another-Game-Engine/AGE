#ifndef  __COLLISION_SHAPE_HPP__
# define __COLLISION_SHAPE_HPP__

#include <MediaFiles/MediaFile.hpp>
#include <vector>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/common.hpp>
#include <cereal/types/list.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>
#include <Utils/GlmSerialization.hpp>

struct CollisionShapeFile : public MediaFile<CollisionShapeFile>
{
	CollisionShapeFile() : MediaFile<CollisionShapeFile>()
	{
		_type = COLLISION_SHAPE;
	}
	virtual ~CollisionShapeFile()
	{}

	CollisionShapeFile(const CollisionShapeFile &o)
		: MediaFile<CollisionShapeFile>(o)
	{
	}

	CollisionShapeFile &operator=(const CollisionShapeFile &o)
	{
		if (&o != this)
		{
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
			init();
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
				init();
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
			ar(name, vertices, normals, colors, uvs, indices);
			init();
		}

		void init()
		{
		}

	};

	std::vector<Geometry> geometries;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new CollisionShapeFile();
		res->manager = manager;
		ar(static_cast<CollisionShapeFile&>(*res));
		return res;
	}

	template <typename Archive>
	void save(Archive &ar) const
	{
		ar(CEREAL_NVP(geometries));
	}

	template <typename Archive>
	void load(Archive &ar)
	{
		ar(geometries);
	}

};



#endif  //__COLLISION_SHAPE_HPP__