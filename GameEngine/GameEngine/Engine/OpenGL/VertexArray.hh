#ifndef VERTEXARRAY_HH_
# define VERTEXARRAY_HH_

# include <vector>
# include <Utils/OpenGL.hh>

# include "VertexBuffer.hh"

namespace OpenGLTools
{
	/// Class representing vertexArray
	class VertexArray
	{
	public:
		struct Attribute
		{
			size_t nbrElement;
			unsigned char nbrComponent;
			unsigned char nbrByte;
			Attribute(size_t nbrElement, unsigned char nbrComponent, unsigned char nbrByte);
			~Attribute();
			Attribute(Attribute const &copy);
			Attribute &operator=(Attribute const &attribute);
		};
	public:
		 VertexArray();
		~VertexArray();
		 VertexArray(VertexArray const &vertexarray);
		 VertexArray &operator=(VertexArray const &vertexarray);

		void addAttribute(size_t nbrElement, unsigned char nbrComponent, unsigned char nbrByte, Byte *data);

		VertexBuffer const &getIndices() const;
		VertexBuffer const &getData() const;

		void setIndices(size_t nbrElement, unsigned int *buffer);
		void clearIndices();
		void setData(VertexBuffer const &vertexbuffer);
		void clearData();
		void transferGPU(GLenum mode)     const;
		void handleError()				  const;
		void draw(GLenum draw)			  const;

	private:
		bool                   _withIndex;
		std::vector<Attribute> _attributes;
		VertexBuffer           _data;
		VertexBuffer           _indices;
		GLuint                 _id;
	};
}

#endif /*!VERTEXARRAY_HH_*/