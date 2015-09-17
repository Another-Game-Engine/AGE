#include "VectorSerialization.hpp"
#include "SerializationArchives.hpp"
#include <glm/glm.hpp>

SERIALIZATION_SAVE_DEFINITION(glm::vec2, *"*/ float x, y; x = v.x; y = v.y; ar(x, y); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::vec2, *"*/ float x, y; ar(x, y); v = glm::vec2(x, y); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::uvec2, *"*/  glm::uint32_t x, y; x = v.x; y = v.y; ar(x, y); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::uvec2, *"*/ glm::uint32_t x, y; ar(x, y); v = glm::uvec2(x, y); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::vec3, *"*/ float x, y, z; x = v.x; y = v.y; z = v.z; ar(x, y, z); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::vec3, *"*/ float x, y, z; ar(x, y, z); v = glm::vec3(x, y, z); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::uvec3, *"*/ glm::uint32_t x, y, z; x = v.x; y = v.y; z = v.z; ar(x, y, z); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::uvec3, *"*/ glm::uint32_t x, y, z; ar(x, y, z); v = glm::uvec3(x, y, z); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::vec4, *"*/ float x, y, z, w; x = v.x; y = v.y; z = v.z; w = v.w; ar(x, y, z, w); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::vec4, *"*/ float x, y, z, w; ar(x, y, z, w); v = glm::vec4(x, y, z, w); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::uvec4, *"*/ glm::uint32_t x, y, z, w; x = v.x; y = v.y; z = v.z; w = v.w; ar(x, y, z, w); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::uvec4, *"*/ glm::uint32_t x, y, z, w; ar(x, y, z, w); v = glm::uvec4(x, y, z, w); /*"*);