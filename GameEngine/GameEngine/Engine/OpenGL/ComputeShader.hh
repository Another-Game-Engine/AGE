#ifndef   __COMPUTE_SHADER_HH__
# define  __COMPUTE_SHADER_HH__

#include <Utils/File.hpp>
#include "AShader.hh"

namespace OpenGLTools
{
	class ComputeShader : public AShader
	{
	public:
		ComputeShader();
		virtual ~ComputeShader();
		bool init(const File &file);
	private:
		virtual bool _build();
	private:
		GLuint _csId;
	};
};

# endif  //__COMPUTE_SHADER_HH__