#include <Render/ProgramResources/BlockResourcesFactory.hh>
#include <Render/ProgramResources/UniformBlock.hh>
#include <Render/ProgramResources/Vec4.hh>
#include <Render/ProgramResources/Mat4.hh>

BlockResourcesFactory::BlockResourcesFactory(UniformBlock const &parent) :
_parent(parent),
_blue_prints({ std::make_pair(GL_FLOAT_VEC4, [&](GLint id){ return (std::make_unique<Vec4>(glm::vec4(0), _parent.program(), id)); }),
std::make_pair(GL_FLOAT_MAT4, [&](GLint id){return (std::make_unique<Mat4>(glm::mat4(0), _parent.program(), id)); }) })
{
}

std::unique_ptr<IBlockResources> BlockResourcesFactory::build(GLenum mode, GLint id)
{
	for (auto &blue_print : _blue_prints) {
		if (blue_print.first == mode) {
			return (blue_print.second(id));
		}
	}
	return (nullptr);
}
