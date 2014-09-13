#include <Core/PrepareElement.hh>
#include <Core/AScene.hh>

namespace AGE
{

	PrepareElement::PrepareElement()
	{

	}

	PrepareElement::~PrepareElement()
	{

	}

	PrepareElement::PrepareElement(PrepareElement &&o)
		: _OTKey(o._OTKey),
		_scene(o._scene)
	{
	}

	PrepareElement &PrepareElement::operator=(PrepareElement &&o)
	{
		_OTKey = o._OTKey;
		_scene = o._scene;
		return *this;
	}

}