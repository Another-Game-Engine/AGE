#include <Render/Properties/Materials/Color.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>

namespace AGE
{
	Color::Color(std::string const &add_name) :
		AProperty(add_name + "_color"),
		_color(glm::vec4(1.0f))
	{

	}

	Color::Color(Color &&move) :
		AProperty(std::move(move)),
		_color(move._color)
	{

	}

	void Color::_update(IProgramResources *res)
	{
		auto resource = (Vec4*)(res);
		*resource = _color;
	}

	glm::vec4 const & Color::get()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_color);
	}

	Color & Color::set(glm::vec4 const &color)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_color = color;
		return (*this);
	}

}
