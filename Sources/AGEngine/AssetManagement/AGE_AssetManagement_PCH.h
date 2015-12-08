// PrecompiledHeader
//------------------------------------------------------------------------------

#include "AssetManager.hh"
#include "OpenGLDDSLoader.hh"

#include "Instance/AnimationInstance.hh"
#include "Instance/MaterialInstance.hh"
#include "Instance/MeshInstance.hh"

#include "Data/AnimationData.hpp"
#include "DataMaterialData.hh"
#include "Data/MeshData.hh"
#include "Data/TextureData.hh"

#include <memory.h>
#if defined( __WINDOWS__ )
	#include <windows.h>
#endif

//------------------------------------------------------------------------------
