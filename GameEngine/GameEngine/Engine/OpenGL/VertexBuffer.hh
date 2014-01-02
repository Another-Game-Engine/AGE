//
// VertexBuffer.hh for  in /home/massora/GIT/amd_project/render/ShaderTool
// 
// Made by dorian pinaud
// Login   <pinaud_d@epitech.net>
// 
// Started on  Sun Jul 28 23:56:28 2013 dorian pinaud
// Last update Mon Aug  5 17:30:46 2013 dorian pinaud
//

#ifndef VERTEXBUFFER_HH_
# define VERTEXBUFFER_HH_

# include "Utils/OpenGL.hh"

# include <iostream>
# include <vector>
# include <map>
# include <string>

# include "Attribute.hh"
# include "UniformBuffer.hh"

namespace OpenGLTools
{
  /// Buffer containing on the GPU the vertex
  class VertexBuffer
  {
  private:
    VertexBuffer &operator=(VertexBuffer const &);
  private:
    GLuint					_idBuffer;
    GLuint					_idIndex;
    size_t					_nbrVertex;
    std::vector<Attribute>	_attribute;
	bool					_drawable;

  public:
    VertexBuffer(void);
    ~VertexBuffer(void);
    VertexBuffer(VertexBuffer const &);


    void addAttribute(Attribute const &attribute);
	Attribute *getAttribute(unsigned int index);
    void clearAttributes(void);
    void clearUniform(void);

    void init(size_t nbrVertex, unsigned int *vertex = NULL);
    void setBuffer(size_t index, byte *buffer) const;

    void draw(GLenum mode) const;
    GLuint getId() const;
  };
}

#endif /*!VERTEXBUFFER_HH_*/
