#ifndef VERTEXBUFFER_HH_
# define VERTEXBUFFER_HH_

# include "Utils/OpenGL.hh"

# include <iostream>
# include <vector>
# include <map>
# include <string>

# include "UniformBuffer.hh"

namespace OpenGLTools
{
<<<<<<< HEAD
  /// Buffer containing on the GPU the vertex
  class VertexBuffer
  {
  private:
    VertexBuffer(VertexBuffer const &);
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

    void addAttribute(Attribute const &attribute);
	Attribute *getAttribute(unsigned int index);
    void clearAttributes(void);
    void clearUniform(void);

    void init(size_t nbrVertex, unsigned int *vertex = NULL);
    void setBuffer(size_t index, byte *buffer) const;

    void draw(GLenum mode) const;
    GLuint getId() const;
  };
=======
	typedef unsigned char Byte;

	/// Buffer containing on the GPU the vertex
	class VertexBuffer
	{
	private:
		struct Buffer
		{
			size_t size;
			Byte *data;

			Buffer(size_t size, Byte *data)
				: size(size), data(data)
			{}
			Buffer(Buffer const &copy)
				: size(copy.size), data(copy.data)
			{}
			~Buffer(){}
		};

	public:
		VertexBuffer();
		~VertexBuffer();
		VertexBuffer(VertexBuffer const &copy);
		
		void init();
		void unload();

		VertexBuffer &operator=(VertexBuffer const &vertexbuffer);
		void pushBuffer(Byte *data, size_t size);
		
		size_t getSize() const;
		Buffer const &getBuffer(size_t index) const;
		void setBuffer(Buffer const &buffer, size_t index);
		
		void popBuffer();
		void clearBuffer();
		
		void transferGPU(bool isIndice, GLenum mode) const;
		void handleError() const;

	private:
		GLuint _id;
		std::vector<Buffer> _buffers;
		size_t _size;
	};
>>>>>>> 2e79899faeb4f4542f69433c05fa9d4f84cb4912
}

#endif /*!VERTEXBUFFER_HH_*/
