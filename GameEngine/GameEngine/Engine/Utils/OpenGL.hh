#ifndef		OPENGL_HEADER_HH_
#define		OPENGL_HEADER_HH_

# include	<GL\glew.h>

#ifdef	WIN32
# ifndef NOMINMAX
#  define NOMINMAX 
# endif
# include	<Windows.h>
# include	<GL\GL.h>
# include	<GL\GLU.h>
# include	<GL\glext.h>
#else
# include	<GL\GL.h>
# include	<GL\GLU.h>
#endif

#endif