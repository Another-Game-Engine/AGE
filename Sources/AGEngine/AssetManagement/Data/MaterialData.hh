#pragma once

# include <string>
# include <glm/glm.hpp>
#include <stdint.h>
#include <Utils/Serialization/SerializationArchives.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{

	struct MaterialData
	{
	public:
		glm::vec4 diffuse = glm::vec4(1.0f);
		glm::vec4 ambient = glm::vec4(1.0f);
		glm::vec4 emissive = glm::vec4(1.0f);
		glm::vec4 reflective = glm::vec4(1.0f);
		glm::vec4 specular = glm::vec4(1.0f);
		std::string diffuseTexPath;
		std::string ambientTexPath;
		std::string emissiveTexPath;
		std::string reflectiveTexPath;
		std::string specularTexPath;
		std::string normalTexPath;
		std::string bumpTexPath;
		// will scale UVs based on the scale of the mesh
		bool scaleUVs = false;
		float shininess = 1.0f;
#ifdef EDITOR_ENABLED
		int selectedTextureIndex[7];
#endif

	public:
		template <class Archive> void serialize(Archive &ar, const std::uint32_t version);
		MaterialData()
		{
#ifdef EDITOR_ENABLED
			for (auto i = 0; i < 7; ++i)
			{
				selectedTextureIndex[i] = 0;
			}
#endif
		}
	};

	struct MaterialDataSet
	{
	public:
		std::vector<MaterialData> collection;
		std::string name;
	public:
		template <class Archive> void serialize(Archive &ar, const std::uint32_t version);
	};

	template <class Archive>
	void MaterialDataSet::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(collection, name);
	}

	template <class Archive>
	void MaterialData::serialize(Archive &ar, const std::uint32_t version)
	{
		ar(diffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath, bumpTexPath);
		if (version > 0)
		{
			ar(scaleUVs);
		}
		if (version > 1)
		{
			ar(shininess);
		}
	}
}

CEREAL_CLASS_VERSION(AGE::MaterialData, 2)
CEREAL_CLASS_VERSION(AGE::MaterialDataSet, 0)