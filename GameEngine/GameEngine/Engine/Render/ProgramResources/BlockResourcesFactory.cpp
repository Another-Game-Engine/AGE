#include <Render/ProgramResources/BlockResourcesFactory.hh>
#include <Render/ProgramResources/Vec4.hh>
#include <Render/ProgramResources/Mat4.hh>

BlockResourcesFactory::BlockResourcesFactory(UniformBlock const &parent) :
_parent(parent),
_blue_prints({ std::make_pair(GL_FLOAT_VEC4, [this](GLint id){ return (std::make_unique<Vec4>(glm::vec4(0), _parent.program(), id)); }),
std::make_pair(GL_FLOAT_MAT4, [this](GLint id){return (std::make_unique<Mat4>(glm::mat4(0), _parent.program(), id)); }) })
{
}

/**
* Method:    operator()
* FullName:  BlockResourcesFactory::operator()
* Access:    public 
* Returns:   std::unique_ptr<IBlockResources>
* Qualifier:
* Parameter: GLenum mode
* Parameter: GLint id
* Parameter: std::string & & name
* Goal:		 Generate the BlockResource wanted
*/
std::unique_ptr<IBlockResources> BlockResourcesFactory::operator()(GLenum mode, GLint id)
{
	for (auto &blue_print : _blue_prints) {
		if (blue_print.first == mode) {
			return (blue_print.second(id));
		}
	}
	return (nullptr);
}
