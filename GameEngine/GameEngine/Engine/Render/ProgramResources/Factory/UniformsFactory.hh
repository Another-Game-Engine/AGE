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
		typedef std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLuint id, std::string &&name)>> create_type_t;
		typedef std::pair<GLenum, std::function<std::shared_ptr<IProgramResources>(GLenum mode, size_t size, size_t stride, GLint id, std::string &&name)>> create_type_array_t;
	
	public:
		UniformsFactory();
		UniformsFactory(UniformsFactory const &copy) = delete;
		UniformsFactory &operator=(UniformsFactory const &p) = delete;

	public:
		std::shared_ptr<IProgramResources> build(GLenum mode, GLint id, std::string &&name);
		std::shared_ptr<IProgramResources> build_array(GLenum mode, size_t size, size_t stride, GLint id, std::string &&name);
		
	private:
		create_type_t _create_vec4() const;
		create_type_t _create_mat4() const;
		create_type_t _create_float() const;
		create_type_t _create_sampler() const;
		create_type_array_t _create_float_array() const;
		create_type_array_t _create_vec4_array() const;
		create_type_array_t _create_mat4_array() const;

	private:
		std::vector<create_type_t> _blue_prints;
		std::vector<create_type_array_t> _blue_prints_array;
	};
}