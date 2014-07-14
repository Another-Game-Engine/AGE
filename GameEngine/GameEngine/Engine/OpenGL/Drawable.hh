#pragma once

#include <glm/glm.hpp>
#include <OpenGL/Key.hh>

namespace gl
{
	class Vertices;
	class Indices;
	class VertexPool;
	class IndexPool;
	class Material;

	//!\file Data.hh
	//!\author Dorian Pinaud
	//!\version v2.0
	//!\class Vertices
	//!\brief use to set the drawing mode
	class Drawable
	{
	public:
		Drawable();
		~Drawable();

		//setter
		Drawable &setModel(glm::mat4 const &m);
		Drawable &setGeometry(Key<Vertices> const &, Key<Indices> const &, Key<VertexPool> const &, Key<IndexPool> const &);
		Drawable &setMaterial(Key<Material> const &);

		//getter
		glm::mat4 const &getModel() const;
		Key<Vertices> const &getVertices() const;
		Key<Indices> const &getIndices() const;
		Key<VertexPool> const &getVertexPool() const;
		Key<IndexPool> const &getIndexPool() const;
		Key<Material> const &getMaterial() const;
	private:

		glm::mat4 _model;
		Key<Vertices> _vertices;
		Key<Indices> _indices;
		Key<VertexPool> _vertexPool;
		Key<IndexPool> _indexPool;
		Key<Material> _material;

	};
}