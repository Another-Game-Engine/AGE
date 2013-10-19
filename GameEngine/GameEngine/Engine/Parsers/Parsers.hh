
#ifndef		PARSERS_HH_
#define		PARSERS_HH_

GLbyte *loadTGA(const char *szFileName,
			   GLint *iWidth, GLint *iHeight,
			   GLint *iComponents, GLenum *eFormat);

bool	loadObj(std::string const &path, Geometry &geometry);

#endif
