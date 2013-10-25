#ifndef		CUBEMAP_HH_
#define		CUBEMAP_HH_

#include "AResource.hh"

#include "Utils/OpenGL.hh"
#include "ResourceManager\AResource.hh"

namespace	Resources
{
	class CubeMap : public AResource
	{
	private:
		GLuint _id;
		GLuint _vbo;
		GLuint _vao;
	public:
		CubeMap(void);
		virtual ~CubeMap(void);

		virtual bool	load(std::string const &path);
		const GLuint	getId() const;
		void draw() const;
	};

}

#endif