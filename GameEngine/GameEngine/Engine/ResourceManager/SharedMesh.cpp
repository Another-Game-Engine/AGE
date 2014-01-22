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
		std::array<Attribute, 4> param =
		{
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 4),
			Attribute(GL_FLOAT, sizeof(float), 2)
		};
	
		_manager = new VertexManager<4>(param);
		_manager->init();
		if (loadObj(path, *this) == false)
			return (false);
		_objs.resize(_geometry.size());
		for (unsigned int i = 0; i < _geometry.size(); ++i)
		{
			std::array<Data, 4> data = { Data(_geometry[i].vertices.size() * 4 * sizeof(float), &_geometry[i].vertices[0].x),
				Data(_geometry[i].colors.size() * 4 * sizeof(float), &_geometry[i].colors[0].x),
				Data(_geometry[i].normals.size() * 4 * sizeof(float), &_geometry[i].normals[0].x),
				Data(_geometry[i].uvs.size() * 2 * sizeof(float), &_geometry[i].uvs[0].x) };
			Data indicesData(_geometry[i].indices.size() * sizeof(unsigned int), &_geometry[i].indices[0]);
			_objs[i] = Vertice<4>(_geometry[i].vertices.size(), data, &indicesData);
			_manager->addVertice(_objs[i]);
			assert(_geometry[i].vertices.size() > 0 && "Cannot create mesh without vertices.");
		}
		return (true);
	}
	
	void		SharedMesh::draw() const
	{
		for (auto &e : _objs)
			e.draw();
	}
	
	std::vector<Geometry>      &SharedMesh::getGeometry()
	{
		return _geometry;
	}

	std::vector<Vertice<4>> const &SharedMesh::getDrawable() const
	{
		return (_objs);
	}
}