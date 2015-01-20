#include "Material.hpp"

SERIALIZATION_SERIALIZE_METHOD_DECLARATION(AGE::MaterialData, SERIALIZATION_CODE_BLOCK(ar(SERIALIZATION_CODE_BLOCKdiffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath, bumpTexPath);))

template <class Archive>
void serialize(Archive &ar)
{
	
}