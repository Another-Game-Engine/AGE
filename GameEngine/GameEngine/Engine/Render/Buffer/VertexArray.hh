#pragma once

# include <Utils/OpenGL.hh>
# include <Render/Buffer/IBuffer.hh>
# include <Render/GeometryManagement/Attribute.hh>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	VertexArray(VertexArray &&move);
	VertexArray(VertexArray const &vertexarray) = delete;
	VertexArray &operator=(VertexArray const &vertexarray) = delete;

public:
	VertexArray const &attribute(size_t index, Attribute type, IBuffer const &buffer) const;
	VertexArray const &attribute(size_t index, Attribute type) const;
	void bind() const;
	void unbind() const;
	GLuint getId() const;

private:
	GLuint _id;
};

