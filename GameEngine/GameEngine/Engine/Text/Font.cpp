#include "Font.hh"

Font::Font()
: _name("")
, _isLoaded(false)
{
}

Font::~Font()
{
}

bool Font::load()
{
	if (_isLoaded)
		return true;
	for (auto &e : _sizes)
	{
		if (!e.second.load())
			return false;
	}
	return true;
}

bool Font::isLoaded()
{
	return _isLoaded;
}