#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <utility>
# include <functional>
# include <memory>
# include <Render/ProgramResources/IProgramResources.hh>

class Program;

class UniformsFactory
{
public:
	UniformsFactory();
	UniformsFactory(UniformsFactory const &copy) = delete;
	UniformsFactory &operator=(UniformsFactory const &p) = delete;

public:
	std::shared_ptr<IProgramResources> build(GLenum mode, GLint id, std::string &&name);

private:
	std::vector<std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLuint id, std::string &&name)>>> _blue_prints;
};