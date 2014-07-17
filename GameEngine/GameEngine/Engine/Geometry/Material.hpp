#pragma once

#include <string>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

namespace AGE
{
	struct MaterialData
	{
		std::string name;

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

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(name, diffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath);
		}
	};

	struct MaterialDataSet
	{
		std::vector<MaterialData> collection;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(collection);
		}
	};

	struct MaterialInstance
	{
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
	};

	struct MaterialSetInstance
	{
		std::map<std::string, MaterialInstance> datas;
	};
}