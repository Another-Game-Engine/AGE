#include <Render/Properties/Transformation.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>

namespace AGE
{

	Transformation::Transformation(glm::mat4 const &model_matrix) :
		AProperty(std::string("model_matrix")),
		_model_matrix(model_matrix)
	{

	}

	Transformation::Transformation(Transformation &&move) :
		AProperty(std::move(move)),
		_model_matrix(move._model_matrix)
	{

	}

	Transformation &Transformation::operator=(glm::mat4 const &other)
	{
		if (_model_matrix != other) {
			_model_matrix = other;
		}
		return (*this);
	}

	IProperty & Transformation::update(std::shared_ptr<Program> const &program)
	{
		auto resource = std::static_pointer_cast<Mat4>(get_resource(program));
		if (resource) {
			*resource = _model_matrix;
		}
		return (*this);
	}

	glm::mat4 const & Transformation::get() const
	{
		return (_model_matrix);
	}

	Transformation & Transformation::set(glm::mat4 const &mat)
	{
		_model_matrix = mat;
		return (*this);
	}
}
