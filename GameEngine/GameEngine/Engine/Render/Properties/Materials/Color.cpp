#include <Render/Properties/Materials/Color.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>

Color::Color(std::string const &add_name) :
AProperty(add_name + "color"),
_color(glm::vec4(1.0f))
{

}

Color::Color(Color &&move) :
AProperty(std::move(move)),
_color(move._color)
{

}

IProperty & Color::update(std::shared_ptr<Program> const &program)
{
	auto &resource = *std::static_pointer_cast<Vec4>(get_resource(program));
	resource = _color;
	return (*this);
}

glm::vec4 const & Color::get() const
{
	return (_color);
}

Color & Color::set(glm::vec4 const &color)
{
	_color = color;
	return (*this);
}

