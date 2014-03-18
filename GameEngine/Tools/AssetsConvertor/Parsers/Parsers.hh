
#ifndef		PARSERS_HH_
#define		PARSERS_HH_

#include "tiny_obj_loader.h"

class File;

GLbyte *loadTGA(const char *szFileName,
			   GLint *iWidth, GLint *iHeight,
			   GLint *iComponents, GLenum *eFormat, std::size_t *size);

#endif
