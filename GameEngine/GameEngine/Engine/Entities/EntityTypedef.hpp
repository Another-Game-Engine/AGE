#pragma once

#include <cstdint>

#define MAX_TAG_NUMBER (32)
#define MAX_CPT_NUMBER (96)
#define MAX_CHILDREN   (32)

typedef std::uint16_t ENTITY_ID;
typedef std::uint8_t  COMPONENT_ID;
typedef std::uint8_t  TAG_ID;
typedef std::uint8_t  ENTITY_VERSION;
typedef std::uint8_t  ENTITY_FLAGS;

#define MAX_ENTITY_NUMBER ((ENTITY_ID)(-1))
