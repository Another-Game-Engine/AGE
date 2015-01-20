#pragma once

#include <string>
#include <glm/fwd.hpp>
//#include <Utils/Serialization/VectorSerialization.hpp>
//#include <cereal/types/vector.hpp>
//#include <cereal/types/string.hpp>
#include <Utils/Serialization/SerializationMacros.hpp>
#include <Render/Material.hh>

SERIALIZATION_ARCHIVE_FORWARD_DECLARATION()

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

		SERIALIZATION_SERIALIZE_METHOD_DECLARATION();
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