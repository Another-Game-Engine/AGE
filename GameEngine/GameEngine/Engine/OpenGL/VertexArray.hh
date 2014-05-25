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
		 VertexArray(VertexArray const &vertexarray);
		 VertexArray &operator=(VertexArray const &vertexarray);
		 void init();
		 void bind();
		 void unbind();
		 GLuint getId() const;
	private:
		GLuint _id;
		bool _isBind;
	};
}

#endif /*!VERTEXARRAY_HH_*/