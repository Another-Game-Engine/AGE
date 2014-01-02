
#ifndef		PARSERS_HH_
#define		PARSERS_HH_

#include "ResourceManager/SharedMesh.hh"
#include "tiny_obj_loader.h"

GLbyte *loadTGA(const char *szFileName,
			   GLint *iWidth, GLint *iHeight,
			   GLint *iComponents, GLenum *eFormat);

bool	loadObj(std::string const &path, std::vector<Resources::Geometry> &geometry);
void    loadObjShape(tinyobj::shape_t &shape, Resources::Geometry &geometry);

#endif
