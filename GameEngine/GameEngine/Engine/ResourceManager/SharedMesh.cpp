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
	_buffer.resize(_geometry.size());
	for (unsigned int i = 0; i < _geometry.size(); ++i)
	{
		_buffer[i].init(_geometry[i].vertices.size(), &_geometry[i].indices[0]);
		_buffer[i].addAttribute(OpenGLTools::Attribute(sizeof(float)* 4, 4, GL_FLOAT));
		_buffer[i].addAttribute(OpenGLTools::Attribute(sizeof(float)* 4, 4, GL_FLOAT));
		_buffer[i].addAttribute(OpenGLTools::Attribute(sizeof(float)* 4, 4, GL_FLOAT));
		_buffer[i].addAttribute(OpenGLTools::Attribute(sizeof(float)* 2, 2, GL_FLOAT));
		assert(_geometry[i].vertices.size() > 0 && "Cannot create mesh without vertices.");
		_buffer[i].setBuffer(0, reinterpret_cast<byte *>(&_geometry[i].vertices[0].x));
		if (_geometry[i].colors.size())
			_buffer[i].setBuffer(1, reinterpret_cast<byte *>(&_geometry[i].colors[0].x));
		if (_geometry[i].normals.size())
			_buffer[i].setBuffer(2, reinterpret_cast<byte *>(&_geometry[i].normals[0].x));
		if (_geometry[i].uvs.size())
			_buffer[i].setBuffer(3, reinterpret_cast<byte *>(&_geometry[i].uvs[0].x));
	}
	return (true);
}

void		SharedMesh::draw() const
{
	for (auto &e : _buffer)
		e.draw(GL_TRIANGLES);
}

std::vector<OpenGLTools::VertexBuffer>	&SharedMesh::getBuffer()
{
	return (_buffer);
}

const std::vector<Geometry>      &SharedMesh::getGeometry() const
{
	return _geometry;
}

}