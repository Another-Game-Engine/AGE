#pragma once

#include <glm/glm.hpp>

namespace gl
{
	//!\file Data.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class Vertices
	//!\brief use to set the drawing mode
	class Drawable
	{
	public:
		Drawable();
		~Drawable();

		glm::mat4 const &getModel() const;
		Drawable &setModel(glm::mat4 const &m);
	private:
		glm::mat4 _model;

	};
}