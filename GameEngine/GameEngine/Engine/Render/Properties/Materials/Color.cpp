#include <Render/Properties/Materials/Color.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>

Color::Color(std::string const &add_name, std::vector<std::shared_ptr<Program>> const &programs) :
AProperty(add_name + "color", programs),
_color(glm::vec4(1.0f))
{

}

Color::Color(std::vector<std::shared_ptr<Program>> const &programs) :
AProperty(std::string("color"), programs),
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

