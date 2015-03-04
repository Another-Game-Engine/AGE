#include <Render/ProgramResources/Factory/UniformsFactory.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>
#include <Render/ProgramResources/Types/Uniform/Array/Mat4Array.hh>

#define LAMBDA_PROTO [this](GLint id, std::string &&name)
#define LAMBDA_PROTO_ARRAY [this](GLint id, size_t size, size_t stride, std::string &&name)

namespace AGE
{

	UniformsFactory::UniformsFactory() :
		_blue_prints({ _create_vec4(), _create_mat4(), _create_float(), _create_sampler() }),
		_blue_prints_array({ _create_vec4_array(), _create_mat4_array(), _create_float_array() })
	{

	}

	std::shared_ptr<IProgramResources> UniformsFactory::build(GLenum mode, GLint id, std::string &&name)
	{
		for (auto &blue_print : _blue_prints) {
			if (mode == blue_print.first) {
				return (blue_print.second(id, std::move(name)));
			}
		}
		return (std::shared_ptr<IProgramResources>(nullptr));
	}
	
	std::shared_ptr<IProgramResources> UniformsFactory::build_array(GLenum mode, size_t size, size_t stride, GLint id, std::string &&name)
	{
		for (auto &blue_print_array : _blue_prints_array) {
			if (mode == blue_print_array.first) {
				return (blue_print_array.second(id, size, stride, std::move(name)));
			}
		}
		return (std::shared_ptr<IProgramResources>(nullptr));
	}

	UniformsFactory::create_type_t UniformsFactory::_create_vec4() const
	{
		return std::make_pair(GL_FLOAT_VEC4, LAMBDA_PROTO {
			return (std::make_shared<Vec4>(glm::vec4(0.0f), id, std::move(name)));
		});
	}

	UniformsFactory::create_type_t UniformsFactory::_create_mat4() const
	{
		return std::make_pair(GL_FLOAT_MAT4, LAMBDA_PROTO {
			return (std::make_shared<Mat4>(glm::mat4(1.0f), id, std::move(name)));
		});
	}

	UniformsFactory::create_type_t UniformsFactory::_create_float() const
	{
		return std::make_pair(GL_FLOAT, LAMBDA_PROTO {
			return (std::make_shared<Vec1>(0.0f, id, std::move(name)));
		});
	}

	UniformsFactory::create_type_t UniformsFactory::_create_sampler() const
	{
		return std::make_pair(GL_SAMPLER_2D, LAMBDA_PROTO {
			return (std::make_shared<Sampler2D>(id, std::move(name)));
		});
	}

	UniformsFactory::create_type_array_t UniformsFactory::_create_float_array() const
	{
		return  std::make_pair(GL_FLOAT, LAMBDA_PROTO_ARRAY {
			return (nullptr);
		});
	}

	UniformsFactory::create_type_array_t UniformsFactory::_create_vec4_array() const
	{
		return  std::make_pair(GL_FLOAT_VEC4, LAMBDA_PROTO_ARRAY{
			return (nullptr);
		});
	}

	UniformsFactory::create_type_array_t UniformsFactory::_create_mat4_array() const
	{
		return  std::make_pair(GL_FLOAT_MAT4, LAMBDA_PROTO_ARRAY{
			return (std::make_shared<Mat4Array>(id, size, stride, std::move(name)));
		});
	}
}