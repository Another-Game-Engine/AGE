#pragma once

#include "SerializationMacros.hpp"
#include <glm/fwd.hpp>

SERIALIZATION_ARCHIVE_FORWARD_DECLARATION()

SERIALIZATION_SAVE_DECLARATION(glm::quat);
SERIALIZATION_LOAD_DECLARATION(glm::quat);
