#include "btSerialization.hpp"

#include "SerializationArchives.hpp"

SERIALIZATION_SAVE_DEFINITION(btVector3, *"*/ float x, y, z; x = v.x(); y = v.y(); z = v.z(); ar(x, y, z); /*"*);
SERIALIZATION_LOAD_DEFINITION(btVector3, *"*/ float x, y, z; ar(x, y, z); v = btVector3(x, y, z); /*"*);
