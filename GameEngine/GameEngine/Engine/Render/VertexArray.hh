#ifndef VERTEXARRAY_HH_
# define VERTEXARRAY_HH_

# include "Utils/OpenGL.hh"

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	void bind() const;
	void unbind() const;
	GLuint getId() const;
private:
	GLuint _id;

	VertexArray(VertexArray const &vertexarray);
	VertexArray &operator=(VertexArray const &vertexarray);
};

#endif /*!VERTEXARRAY_HH_*/