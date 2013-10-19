//
// Attribute.hh for  in /home/massora/GIT/amd_project/render/ShaderTool
// 
// Made by dorian pinaud
// Login   <pinaud_d@epitech.net>
// 
// Started on  Fri Jul 26 18:14:06 2013 dorian pinaud
// Last update Wed Aug  7 01:20:00 2013 dorian pinaud
//

#ifndef ATTRIBUTE_HH_
# define ATTRIBUTE_HH_

# include "Utils/OpenGL.hh"

# include <set>

typedef unsigned char byte;

namespace OpenGLTools
{
  struct Attribute
  {
    size_t nbrByte;
    size_t nbrCompenent;
    size_t offset;
    GLenum type;
    Attribute(size_t nbrByte, size_t nbrCompenent, GLenum type);
    ~Attribute(void);
    Attribute(Attribute const &copy);
    Attribute &operator=(Attribute const &r);
  };
};

#endif /*!ATTRIBUTE_HH_*/
