#include <Render/ProgramResources/BlockResourcesFactory.hh>
#include <Render/ProgramResources/Vec4.hh>
#include <Render/ProgramResources/Mat4.hh>

BlockResourcesFactory::BlockResourcesFactory() :
_blue_prints({ std::make_pair(GL_FLOAT_VEC4, [](GLint id, std::string &&name){return (std::make_unique<Vec4>(Vec4(glm::vec4(0), id, std::move(name)))); }),
std::make_pair(GL_FLOAT_MAT4, [](GLint id, std::string &&name){return (std::make_unique<Mat4>(Mat4(glm::mat4(0), id, std::move(name)))); }) })
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
std::unique_ptr<IBlockResources> BlockResourcesFactory::operator()(GLenum mode, GLint id, std::string &&name)
{
	for (auto &blue_print : _blue_prints) {
		if (blue_print.first == mode) {
			return (blue_print.second(id, std::move(name)));
		}
	}
	return (nullptr);
}
