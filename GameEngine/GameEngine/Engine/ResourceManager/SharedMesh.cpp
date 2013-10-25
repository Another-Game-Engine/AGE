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
	_buffer.init(_geometry.indices.size(), &_geometry.indices[0]);
	_buffer.addAttribute(OpenGLTools::Attribute(sizeof(float) * 4, 4, GL_FLOAT));
	_buffer.addAttribute(OpenGLTools::Attribute(sizeof(float) * 4, 4, GL_FLOAT));
	_buffer.addAttribute(OpenGLTools::Attribute(sizeof(float) * 4, 4, GL_FLOAT));
	_buffer.addAttribute(OpenGLTools::Attribute(sizeof(float) * 2, 2, GL_FLOAT));
	assert(_geometry.vertices.size() > 0 && "Cannot create mesh without vertices.");
	_buffer.setBuffer(0, reinterpret_cast<byte *>(&_geometry.vertices[0].x));
	if (_geometry.colors.size())
	  _buffer.setBuffer(1, reinterpret_cast<byte *>(&_geometry.colors[0].x));
	if (_geometry.normals.size())
	  _buffer.setBuffer(2, reinterpret_cast<byte *>(&_geometry.normals[0].x));
	if (_geometry.uvs.size())
	  _buffer.setBuffer(3, reinterpret_cast<byte *>(&_geometry.uvs[0].x));
	return (true);
}

void		SharedMesh::draw() const
{
	_buffer.draw(GL_TRIANGLES);
}

OpenGLTools::VertexBuffer	&SharedMesh::getBuffer()
{
	return (_buffer);
}

}