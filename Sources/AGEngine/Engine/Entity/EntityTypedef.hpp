#pragma once

#include <cstdint>

namespace AGE
{

#define MAX_TAG_NUMBER (32)
#define MAX_CPT_NUMBER (96)
#define MAX_CHILDREN   (32)
#define ENTITY_NAME_LENGTH (128)

	typedef std::uint16_t ENTITY_ID;
	typedef std::uint8_t  TAG_ID;
	typedef std::uint8_t  ENTITY_VERSION;
	typedef std::uint8_t  ENTITY_FLAGS;
	typedef std::uint32_t ComponentType;

	typedef std::uint8_t    uint8;
	typedef std::uint16_t   uint16;
	typedef std::uint32_t   uint32;
	typedef std::uint64_t   uint64;

#define MAX_ENTITY_NUMBER ((ENTITY_ID)(-1))

}