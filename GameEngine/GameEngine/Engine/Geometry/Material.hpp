#pragma once

#include <string>
#include <glm/glm.hpp>
#include <Utils/GlmSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <Render/Material.hh>

namespace AGE
{
	struct MaterialData
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
		std::string bumpTexPath;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(diffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath, bumpTexPath);
		}
	};

	struct MaterialDataSet
	{
		AGE::Vector<MaterialData> collection;
		std::string name;

		template <class Archive>
		void serialize(Archive &ar)
		{
			ar(collection, name);
		}
	};

	typedef gl::Key<gl::Material> MaterialInstance;

	struct MaterialSetInstance
	{
		AGE::Vector<MaterialInstance> datas;
		std::string name;
		std::string path;
	};
}