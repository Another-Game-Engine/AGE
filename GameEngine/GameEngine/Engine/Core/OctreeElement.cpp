#include <Core/OctreeElement.hh>
#include <Core/AScene.hh>

namespace AGE
{
	OctreeElement::OctreeElement()
	{

	}

	OctreeElement::~OctreeElement()
	{

	}

	OctreeElement::OctreeElement(OctreeElement &&o)
		: _OTKey(o._OTKey),
		_scene(o._scene)
	{
	}

	OctreeElement &OctreeElement::operator=(OctreeElement &&o)
	{
		_OTKey = o._OTKey;
		_scene = o._scene;
		return *this;
	}
}