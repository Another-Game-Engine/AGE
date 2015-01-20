#include "Bone.hpp"
#include <Utils/Serialization/SerializationArchives.hpp>
#include <Utils/Serialization/MatrixSerialization.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/string.hpp>

SERIALIZATION_SERIALIZE_DEFINITION(AGE::Bone, *"*/ ar(cereal::make_nvp(MACRO_STR(name), v.name)); ar(cereal::make_nvp(MACRO_STR(offset), v.offset)); ar(cereal::make_nvp(MACRO_STR(index), v.index)); ar(cereal::make_nvp(MACRO_STR(transformation), v.transformation)); ar(cereal::make_nvp(MACRO_STR(children), v.children)); ar(cereal::make_nvp(MACRO_STR(parent), v.parent)); /*"* );