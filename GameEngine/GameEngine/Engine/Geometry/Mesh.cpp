#include "Mesh.hpp"
#include <Utils/Serialization/SerializationArchives.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>
#include <cereal/types/bitset.hpp>
#include <Utils/Serialization/VectorSerialization.hpp>

SERIALIZATION_SERIALIZE_METHOD_DEFINITION(AGE::SubMeshData, *"*/ ar(name, infos, positions, normals, tangents, biTangents, uvs, indices, weights, boneIndices, colors, boundingBox, defaultMaterialIndex); /*"*);


SERIALIZATION_SERIALIZE_METHOD_DEFINITION(AGE::MeshData, *"*/ ar(name, subMeshs); /*"*);
