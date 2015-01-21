#include "MatrixSerialization.hpp"
#include "SerializationArchives.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

SERIALIZATION_SAVE_DEFINITION(glm::mat2, *"*/ float dArray[8] = { 0.0 }; const float *pSource = (const float*)glm::value_ptr(v); for (int i = 0; i < 8; ++i) { dArray[i] = pSource[i]; }; ar(dArray); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::mat2, *"*/ float source[8] = { 0.0 }; ar(source); v = glm::make_mat2(source); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::mat3, *"*/ float dArray[12] = { 0.0 }; const float *pSource = (const float*)glm::value_ptr(v); for (int i = 0; i < 12; ++i) { dArray[i] = pSource[i]; }ar(dArray); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::mat3, *"*/ float source[12] = { 0.0 }; ar(source); v = glm::make_mat3(source); /*"*);
SERIALIZATION_SAVE_DEFINITION(glm::mat4, *"*/ float dArray[16] = { 0.0 }; const float *pSource = (const float*)glm::value_ptr(v); for (int i = 0; i < 16; ++i) { dArray[i] = pSource[i] + 0.0f; }; ar(dArray); /*"*);
SERIALIZATION_LOAD_DEFINITION(glm::mat4, *"*/ float source[16] = { 0.0 }; ar(source); v = glm::make_mat4(source); /*"*);