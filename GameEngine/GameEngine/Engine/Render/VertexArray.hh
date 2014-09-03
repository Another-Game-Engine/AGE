#ifndef VERTEXARRAY_HH_
# define VERTEXARRAY_HH_

# include "Utils/OpenGL.hh"

namespace gl
{
	/// Class representing vertexArray
	class VertexArray
	{
	public:
		 VertexArray();
		~VertexArray();
		 void bind() const;
		 void unbind() const;
		 GLuint getId() const;
		 VertexArray const &activateAttribute(size_t index) const;
		 VertexArray const &attribute(size_t index, int nbrComponent, GLenum type, size_t offset) const;
	private:
		GLuint _id;

		VertexArray(VertexArray const &vertexarray);
		VertexArray &operator=(VertexArray const &vertexarray);
	};
}

#endif /*!VERTEXARRAY_HH_*/