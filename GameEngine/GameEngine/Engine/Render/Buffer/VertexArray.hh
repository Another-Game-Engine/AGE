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
	void bind();
	void unbind();
	GLuint getId() const;
	static void Unbind();
private:
	GLuint _id;
	static VertexArray *_lastBinded;
};

