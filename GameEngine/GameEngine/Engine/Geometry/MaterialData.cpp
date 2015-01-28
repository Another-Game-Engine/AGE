#include "MaterialData.hpp"
#include <Utils/Serialization/SerializationArchives.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

SERIALIZATION_SERIALIZE_METHOD_DEFINITION(AGE::MaterialData, *"*/ ar(diffuse, ambient, emissive, reflective, specular, diffuseTexPath, ambientTexPath, emissiveTexPath, reflectiveTexPath, specularTexPath, normalTexPath, bumpTexPath); /*"*);

SERIALIZATION_SERIALIZE_METHOD_DEFINITION(AGE::MaterialDataSet, *"*/ ar(collection, name); /*"*);