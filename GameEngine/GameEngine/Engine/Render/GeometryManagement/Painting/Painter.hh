#pragma once

# include <memory>
# include <vector>
# include <Render/Key.hh>
# include <Render/Program.hh>
# include <Render/ProgramResources/IProgramResources.hh>
# include <Render/ProgramResources/Types/Attribute.hh>
# include <Render/Buffer/VertexArray.hh>

class Painter
{
public:
	Painter(std::shared_ptr<Program> const &program);

private:
	typedef std::vector<Key<ProgramResource>> Attributes_key;

private:
	std::shared_ptr<Program> _program;
	Attributes_key _attributes_key;
	VertexArray _vertex_arrays;
};