#include "SharedMesh.hh"

#include "Parsers/Parsers.hh"

#include <assert.h>

namespace	Resources
{

SharedMesh::SharedMesh(void)
{
}

SharedMesh::~SharedMesh(void)
{
}

bool	SharedMesh::load(std::string const &path)
{
	if (loadObj(path, _geometry) == false)
		return (false);
	_buffer.init();
	_buffer.setIndices(_geometry.indices.size(), &_geometry.indices[0]);
	_buffer.addAttribute(_geometry.vertices.size(), 4, sizeof(float), reinterpret_cast<OpenGLTools::Byte *>(&_geometry.vertices[0].x));
	_buffer.addAttribute(_geometry.colors.size(), 4, sizeof(float), reinterpret_cast<OpenGLTools::Byte *>(&_geometry.colors[0].x));
	_buffer.addAttribute(_geometry.normals.size(), 4, sizeof(float), reinterpret_cast<OpenGLTools::Byte *>(&_geometry.normals[0].x));
	_buffer.addAttribute(_geometry.uvs.size(), 2, sizeof(float), reinterpret_cast<OpenGLTools::Byte *>(&_geometry.uvs[0].x));
	_buffer.transferGPU(GL_STREAM_DRAW);
	assert(_geometry.vertices.size() > 0 && "Cannot create mesh without vertices.");
	return (true);
}

void		SharedMesh::draw() const
{
	_buffer.draw(GL_TRIANGLES);
}

OpenGLTools::VertexArray	&SharedMesh::getBuffer()
{
	return (_buffer);
}

}