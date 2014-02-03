#ifndef ERRORCONTROLLER_HH_
# define ERRORCONTROLLER_HH_

# include "Utils/OpenGL.hh"

enum GLERROR
{
	ErrorDraw
};

namespace OpenGLTools
{

	template <GLERROR T>
	void ErrorController();

	template <>
	void ErrorController<ErrorDraw>()
	{
		GLenum error = glGetError();
		if (error == GL_NO_ERROR)
			return;
		std::cerr << "**** Error generate into the draw" << std::endl;
		switch (error)
		{
		case GL_INVALID_VALUE:
			std::cerr << "GL_INVALID_VALUE is generated if count is negative." << std::endl;
			break;
		case GL_INVALID_ENUM:
			std::cerr << "GL_INVALID_ENUM is generated if mode is not an accepted value." << std::endl;
			break;
		case GL_INVALID_OPERATION:
			std::cerr << "GL_INVALID_OPERATION is generated if a geometry shader is active and mode is incompatible with the input primitive type of the geometry shader in the currently installed program object." << std::endl;
			std::cerr << "GL_INVALID_OPERATION is generated if a non-zero buffer object name is bound to an enabled array or the element array and the buffer object's data store is currently mapped." << std::endl;
			break;
		}
	}
}

#endif /*!ERRORCONTROLLER_HH_*/