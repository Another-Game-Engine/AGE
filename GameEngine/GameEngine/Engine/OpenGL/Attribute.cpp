//
// Attribute.cpp for  in /home/massora/GIT/amd_project/render/ShaderTool
// 
// Made by dorian pinaud
// Login   <pinaud_d@epitech.net>
// 
// Started on  Mon Jul 29 19:28:55 2013 dorian pinaud
// Last update Wed Aug  7 01:19:35 2013 dorian pinaud
//

#include "Attribute.hh"

namespace OpenGLTools
{

Attribute::Attribute(size_t nbrByte, size_t nbrCompenent, GLenum type)
  : nbrByte(nbrByte),
    nbrCompenent(nbrCompenent),
    offset(0),
    type(type)
{
}

Attribute::~Attribute(void)
{
}

Attribute::Attribute(Attribute const &copy)
  : nbrByte(copy.nbrByte),
    nbrCompenent(copy.nbrCompenent),
    offset(copy.offset),
    type(copy.type)
{
}

Attribute &Attribute::operator=(Attribute const &r)
{
  nbrByte = r.nbrByte;
  nbrCompenent = r.nbrCompenent;
  offset = r.offset;
  type = r.type;
  return (*this);
}

}