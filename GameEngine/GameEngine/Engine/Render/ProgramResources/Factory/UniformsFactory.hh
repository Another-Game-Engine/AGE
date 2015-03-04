#pragma once

# include <Utils/OpenGL.hh>
# include <vector>
# include <utility>
# include <functional>
# include <memory>
# include <Render/ProgramResources/IProgramResources.hh>

namespace AGE
{
	class Program;

	class UniformsFactory
	{
	public:

	public:
		UniformsFactory();
		UniformsFactory(UniformsFactory const &copy) = delete;
		UniformsFactory &operator=(UniformsFactory const &p) = delete;

	public:
		std::shared_ptr<IProgramResources> build(GLenum mode, GLint id, std::string &&name);
		std::shared_ptr<IProgramResources> build_array(GLenum mode, size_t size, size_t stride, GLint id, std::string &&name);
		
	private:


	private:
		std::vector<std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLuint id, std::string &&name)>>> _blue_prints;
	};
}