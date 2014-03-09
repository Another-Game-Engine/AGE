#include "Font.hh"

Font::Font()
: _name("")
, _isLoaded(false)
{
}

Font::~Font()
{
}

bool Font::load(std::unique_ptr<VertexManager<2>> &vm)
{
	if (_isLoaded)
		return true;
	for (auto &e : _sizes)
	{
		if (!e.second.load(vm))
			return false;
	}
	_isLoaded = true;
	return true;
}

bool Font::isLoaded()
{
	return _isLoaded;
}