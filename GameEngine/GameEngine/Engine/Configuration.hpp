#pragma once

////////////////
/// Configuration file for AGE

namespace AGE
{
// Comment to disable default configuration
// Default configuration will create at the engine
// initialization all the basics dependencies (scene manager, assets manager, ...)
#define USE_DEFAULT_ENGINE_CONFIGURATION

// Comment to disable occlusion culling
#define OCCLUSION_CULLING

#define RESOLUTION_SHADOW_X 1280
#define RESOLUTION_SHADOW_Y 720

}