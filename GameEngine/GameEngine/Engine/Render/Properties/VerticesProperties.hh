#pragma once

# include <glm/glm.hpp>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class VerticesProperties
{
public:
	VerticesProperties();
	VerticesProperties(VerticesProperties const &copy);
	VerticesProperties(VerticesProperties &&move);

public:
	glm::mat4 const &transformation() const;
	VerticesProperties &transformation(glm::mat4 const &transf);
	VerticesProperties &apply(Program &program);
};