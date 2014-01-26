
#ifndef		PARSERS_HH_
#define		PARSERS_HH_

#include "ResourceManager/SharedMesh.hh"
#include "tiny_obj_loader.h"

class File;

GLbyte *loadTGA(const char *szFileName,
			   GLint *iWidth, GLint *iHeight,
			   GLint *iComponents, GLenum *eFormat, unsigned int *size);

void    loadObjShape(tinyobj::shape_t &shape, Resources::Geometry &geometry);
void    loadObjMaterials(tinyobj::shape_t &shape, Resources::SharedMesh &m, const File &objFile, unsigned int index);

#endif
