#ifndef		TEXTURE_HH_
#define		TEXTURE_HH_

#include "AResource.hh"

#include "Utils/OpenGL.hh"
#include "ResourceManager\AResource.hh"

namespace	Resources
{

class Texture : public AResource
{
private:
	GLuint		_id;

	GLenum		_wrap;
	GLenum		_minFilter;
	GLenum		_magFilter;

public:
	Texture(void);
	virtual ~Texture(void);

	void			setWrapMode(GLenum wrapMode);
	void			setFilters(GLenum magFilter, GLenum minFilter);

	virtual bool	load(std::string const &path);

	const GLuint			getId() const;
};

}

#endif