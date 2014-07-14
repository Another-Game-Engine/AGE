#include <OpenGL/Drawable.hh>
#include <OpenGL/Data.hh>
#include <OpenGL/Pool.hh>

namespace gl
{
	Drawable::Drawable()
	{

	}

	Drawable::~Drawable()
	{

	}

	glm::mat4 const &Drawable::getModel() const
	{
		return (_model);
	}

	Drawable &Drawable::setModel(glm::mat4 const &m)
	{
		_model = m;
		return (*this);
	}

	Drawable &Drawable::setGeometry(Key<Vertices> const &vertices, Key<Indices> const &indices, Key<VertexPool> const &vertexPool, Key<IndexPool> const &indexPool)
	{
		_vertices = vertices;
		_indices = indices;
		_vertexPool = vertexPool;
		_indexPool = indexPool;
		return (*this);
	}

	Key<IndexPool> const &Drawable::getIndexPool() const
	{
		return (_indexPool);
	}

	Key<VertexPool> const &Drawable::getVertexPool() const
	{
		return (_vertexPool);
	}

	Key<Vertices> const &Drawable::getVertices() const
	{
		return (_vertices);
	}

	Key<Indices> const &Drawable::getIndices() const
	{
		return (_indices);
	}

	Key<Material> const &Drawable::getMaterial() const
	{
		return (_material);
	}

	Drawable &Drawable::setMaterial(Key<Material> const &m)
	{
		_material = m;
		return (*this);
	}
}