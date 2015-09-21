#include <Render/Properties/Transformation.hh>
#include <Render/ProgramResources/Types/Uniform/Mat4.hh>
#include <Render/ProgramResources/Types/Uniform/Sampler/SamplerBuffer.hh>
#include <Render/Textures/TextureBuffer.hh>
#include <glm/gtc/type_ptr.hpp>
#include <mutex>

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

	void Transformation::update(Mat4 *res, Transformation *trans)
	{
		*res = trans->_model_matrix;
	}

	void Transformation::instanciedUpdate(SamplerBuffer *res, Transformation *trans)
	{
		res->getTexture()->push(glm::value_ptr(trans->_model_matrix));
	}

	glm::mat4 const & Transformation::get()
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		return (_model_matrix);
	}

	Transformation & Transformation::set(glm::mat4 const &mat)
	{
		std::lock_guard<AGE::SpinLock> lock(_mutex);
		_model_matrix = mat;
		return (*this);
	}
}
