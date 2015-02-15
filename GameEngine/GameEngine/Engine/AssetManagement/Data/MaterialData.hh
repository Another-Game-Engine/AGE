#pragma once

# include <string>
# include <glm/glm.hpp>

namespace AGE
{

	struct MaterialData
	{
	public:
		glm::vec4 diffuse;
		glm::vec4 ambient;
		glm::vec4 emissive;
		glm::vec4 reflective;
		glm::vec4 specular;
		std::string diffuseTexPath;
		std::string ambientTexPath;
		std::string emissiveTexPath;
		std::string reflectiveTexPath;
		std::string specularTexPath;
		std::string normalTexPath;
		std::string bumpTexPath;

	public:
		template <class Archive> void serialize(Archive &ar);
	};

	struct MaterialDataSet
	{
	public:
		std::vector<MaterialData> collection;
		std::string name;

	public:
		template <class Archive> void serialize(Archive &ar);
	};

	template <class Archive>
	void MaterialDataSet::serialize(Archive &ar)
	{
		ar(collection, name);
	}

	template <class Archive>
	void MaterialData::serialize(Archive &ar)
	{
		ar(diffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath, bumpTexPath);
	}

}