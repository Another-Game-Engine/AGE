
#ifndef		PARSERS_HH_
#define		PARSERS_HH_

#include "ResourceManager/SharedMesh.hh"

GLbyte *loadTGA(const char *szFileName,
			   GLint *iWidth, GLint *iHeight,
			   GLint *iComponents, GLenum *eFormat);

bool	loadObj(std::string const &path, Resources::Geometry &geometry);

#endif
