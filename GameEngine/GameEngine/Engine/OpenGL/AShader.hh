#ifndef   __ASHADER_HH__
# define  __ASHADER_HH__

# include "Utils/OpenGL.hh"
# include "OpenGL/UniformBuffer.hh"
# include "glm/glm.hpp"

# include <iostream>
# include <fstream>
# include <string>
#include <memory>


namespace OpenGLTools
{
	class AShader
	{
	public:
		AShader();
		virtual ~AShader();
	protected:
	};
};
#endif    //__ASHADER_HH__
