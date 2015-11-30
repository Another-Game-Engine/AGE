#pragma once

#define STRINGID_RT_HASH_ENABLED    1
#define STRINGID_SUPPORT_STD_STRING 1
#ifdef  RETAIL
    #define STRINGID_DEBUG_ENABLED      0
    #define STRINGID_CHECK_COLLISION    0
#else
    #define STRINGID_DEBUG_ENABLED      1
    #define STRINGID_CHECK_COLLISION    1
#endif
#define STRINGID_DB_MAP_INCLUDE     <unordered_map>
#define STRINGID_DB_MAP             std::unordered_map
#define STRINGID_64                 1

#include <StringID/StringID.hpp>