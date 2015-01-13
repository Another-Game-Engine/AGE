#include <Render/ProgramResources/Factory/UniformsFactory.hh>
#include <Render/ProgramResources/Types/Uniform/Vec4.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Vec1.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/Sampler2D.hh>

#define LAMBDA_PROTO [this](GLint id, std::string &&name)

# define DECLAR_BUILDERS \
std::make_pair(GL_FLOAT_VEC4, LAMBDA_PROTO\
{ \
	return (std::make_shared<Vec4>(glm::vec4(0.0f), id, std::move(name))); \
}), \
std::make_pair(GL_FLOAT_MAT4, LAMBDA_PROTO\
{ \
	return (std::make_shared<Mat4>(glm::mat4(1.0f), id, std::move(name))); \
}), \
std::make_pair(GL_FLOAT, LAMBDA_PROTO\
{ \
	return (std::make_shared<Vec1>(0.0f, id, std::move(name))); \
}), \
std::make_pair(GL_SAMPLER_2D, LAMBDA_PROTO\
{\
	return (std::make_shared<Sampler2D>(id, std::move(name))); \
})

UniformsFactory::UniformsFactory() :
_blue_prints({ DECLAR_BUILDERS })
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



