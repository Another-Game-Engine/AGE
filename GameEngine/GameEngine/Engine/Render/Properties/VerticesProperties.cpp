#include <Render/Properties/VerticesProperties.hh>
#include <Render/Program.hh>

VerticesProperties::VerticesProperties()
{

}

VerticesProperties::VerticesProperties(VerticesProperties const &copy)
{

}

VerticesProperties::VerticesProperties(VerticesProperties &&move)
{
}

glm::mat4 const & VerticesProperties::transformation() const
{
	return (glm::mat4(1.0f));
}

VerticesProperties & VerticesProperties::transformation(glm::mat4 const &transf)
{
	return (*this);
}

VerticesProperties & VerticesProperties::apply(Program &program)
{
	return (*this);
}

