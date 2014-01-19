#ifndef  __OBJ_FILE_HPP__
# define __OBJ_FILE_HPP__

#include <MediaFiles/AMediaFile.hpp>
#include <vector>
#include <glm/glm.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/complex.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/archives/json.hpp>
#include <cereal/archives/binary.hpp>
#include <cereal/types/base_class.hpp>
#include <cereal/types/memory.hpp>

#include <glm/gtc/packing.hpp>


//template<typename Archive>
//void serialize(Archive &ar, glm::vec2 &v)
//{
//	ar(v.x, v.y);
//}

template<typename Archive>
void save(Archive &ar, const glm::vec3 &v)
{
	unsigned int t = glm::packF2x11_1x10(v);
	ar(v.x);
}

template<typename Archive>
void load(Archive &ar, glm::vec3 &v)
{
	unsigned int p;
	ar(p);
	v = glm::unpackF2x11_1x10(p);
}


//template<typename Archive>
//void serialize(Archive &ar, glm::vec4 &v)
//{
//	ar(v.x, v.y, v.z, v.w);
//}

struct ObjFile : public AMediaFile
{
	ObjFile() : AMediaFile()
	{
		type = typeid(ObjFile).hash_code();
	}
	virtual ~ObjFile(){}
	struct Geometry
	{
		std::string                 name;
		std::vector<glm::vec4>		vertices;	// vertices positions
		std::vector<glm::vec4>		normals;	// normals
		std::vector<glm::vec4>		colors;		// vertices colors
		std::vector<glm::vec2>		uvs;		// texture coordinates
		std::vector<unsigned int>	indices;	// indices

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(name);
		}
	};
	std::vector<Geometry> geometries;
	glm::vec3 test;
	//	std::vector<MaterialFile> materials;

	virtual AMediaFile *unserialize(cereal::JSONInputArchive &ar)
	{
		return unserialize<cereal::JSONInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::BinaryInputArchive &ar)
	{
		return unserialize<cereal::BinaryInputArchive>(ar);
	}

	virtual AMediaFile *unserialize(cereal::XMLInputArchive &ar)
	{
		return unserialize<cereal::XMLInputArchive>(ar);
	}

	virtual void _serialize(cereal::JSONOutputArchive &ar)
	{
		ar(*this);
	}

	virtual void _serialize(cereal::BinaryOutputArchive &ar)
	{
		ar(*this);
	}

	virtual void _serialize(cereal::XMLOutputArchive &ar)
	{
		ar(*this);
	}

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		ar(static_cast<ObjFile&>(*res));
		return res;
	}

	template <typename Archive>
	void serialize(Archive &ar)
	{
		ar(geometries, test);
	}

	//template <typename Archive>
	//void save(Archive &ar) const
	//{
	//	uint32_t lol = glm::packF2x11_1x10(test);
	//	ar(geometries, lol);
	//}

	//template <typename Archive>
	//void load(Archive &ar)
	//{
	//	uint32_t lol;
	//	ar(geometries, lol);
	//	test = glm::unpackF2x11_1x10(lol);
	//}

};

#endif   //__OBJ_FILE_HPP__