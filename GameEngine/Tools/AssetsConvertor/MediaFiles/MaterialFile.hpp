#ifndef   __MATERIAL_FILE_HPP__
# define  __MATERIAL_FILE_HPP__

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

struct MaterialFile : public MediaFile<MaterialFile>
{
	MaterialFile() : MediaFile<MaterialFile>()
	{
	}
	virtual ~MaterialFile(){}

	struct Material
	{
		std::string name;
		glm::vec3 ambient;
		glm::vec3 diffuse;
		glm::vec3 specular;
		glm::vec3 transmittance;
		glm::vec3 emission;
		float shininess;
		std::string ambientTex;
		std::string diffuseTex;
		std::string specularTex;
		std::string normalTex;
		std::map<std::string, glm::vec2> paramVec2;
		std::map<std::string, glm::vec3> paramVec3;
		std::map<std::string, glm::vec4> paramVec4;
		std::map<std::string, glm::mat2> paramMat2;
		std::map<std::string, glm::mat3> paramMat3;
		std::map<std::string, glm::mat4> paramMat4;
		std::map<std::string, int> paramInt;
		std::map<std::string, float> paramFloat;

		template <typename Archive>
		void serialize(Archive &ar)
		{
			ar(name, ambient, diffuse, specular, transmittance, emission, shininess, ambientTex, diffuseTex, specularTex, normalTex, paramVec2, paramVec3, paramVec4, paramMat2, paramMat3, paramMat4, paramInt, paramFloat);
		}
	};

	std::vector<Material> materials;

	template <typename Archive>
	AMediaFile *unserialize(Archive &ar)
	{
		AMediaFile *res = new ObjFile();
		ar(static_cast<MaterialFile&>(*res));
		return res;
	}

	template <typename Archive>
	void serialize(Archive &ar)
	{
		ar(materials);
	}
};


#endif    //__MATERIAL_FILE_HPP__