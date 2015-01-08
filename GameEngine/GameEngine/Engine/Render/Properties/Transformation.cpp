#include <Render/Properties/Transformation.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>

Transformation::Transformation(std::vector<std::shared_ptr<Program>> const &programs) :
Transformation(glm::mat4(1.0f), programs)
{

}

Transformation::Transformation(glm::mat4 const &model_matrix, std::vector<std::shared_ptr<Program>> const &programs) :
AProperty(std::string("model_matrix"), programs),
_model_matrix(model_matrix)
{

}

Transformation::Transformation(Transformation &&move) :
AProperty(std::move(move)),
_model_matrix(move._model_matrix)
{

}

IProperty & Transformation::update(std::shared_ptr<Program> const &p)
{
	auto resource = get_resource(p);
	*static_cast<Mat4 *>(resource) = _model_matrix;
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

