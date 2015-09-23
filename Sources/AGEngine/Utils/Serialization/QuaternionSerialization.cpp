#include "QuaternionSerialization.hpp"
#include "SerializationArchives.hpp"
#include <glm/gtc/quaternion.hpp>

SERIALIZATION_SAVE_DEFINITION(glm::quat, *"*/ float x = v.x; float y = v.y; float z = v.z; float w = v.w; ar(x, y, z, w); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::quat, *"*/ float x, y, z, w; ar(x, y, z, w); v = glm::quat(w, x, y, z); /*"*);