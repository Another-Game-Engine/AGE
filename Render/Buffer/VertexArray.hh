#pragma once

# include <Utils/OpenGL.hh>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	VertexArray(VertexArray &&move);
	VertexArray(VertexArray const &vertexarray) = delete;
	VertexArray &operator=(VertexArray const &vertexarray) = delete;

public:
	void bind() const;
	void unbind() const;
	GLuint getId() const;

private:
	GLuint _id;
	static GLuint _current_id;
};

