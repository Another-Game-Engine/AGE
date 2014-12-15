#include <Render/ProgramResources/ProgramResourcesFactory.hh>
#include <Render/ProgramResources/Vec4.hh>
#include <Render/ProgramResources/Mat4.hh>

ProgramResourcesFactory::ProgramResourcesFactory(Program const &parent) :
_parent(parent),
_blue_prints({ std::make_pair(GL_FLOAT_VEC4, [this](std::string &&name){ return (std::make_unique<Vec4>(glm::vec4(0.0f), _parent, std::move(name))); }),
std::make_pair(GL_FLOAT_MAT4, [this](std::string &&name){ return (std::make_unique<Mat4>(glm::mat4(1.0f), _parent, std::move(name))); })
})
{

}

std::unique_ptr<IProgramResources> ProgramResourcesFactory::build(GLenum mode, std::string &&name)
{
	for (auto &blue_print : _blue_prints) {
		if (mode == blue_print.first) {
			return (blue_print.second(std::move(name)));
		}
	}
	return (std::unique_ptr<IProgramResources>(nullptr));
}



