#pragma once

#include <Utils/OpenGL.hh>
#include <Render/Key.hh>
#include <Render/Buffer.hh>
#include <Render/VertexArray.hh>
#include <Render/Pool.hh>
#include <Utils/Dependency.hpp>
#include <map>
#include <Utils/Containers/Vector.hpp>
#include <glm/glm.hpp>

namespace gl
{
	class Vertices {};
	class Indices {};
	class VertexPool;
	class IndexPool;

	enum SimpleForm
	{
		QUAD = 0,
		SPHERE,
		NBR_SIMPLE_FORM
	};

	class GeometryManager
	{
	public:
		GeometryManager();
		~GeometryManager();
		GeometryManager &createQuadSimpleForm();
		GeometryManager &createSphereSimpleForm();
		Key<Vertices> getSimpleFormGeo(SimpleForm form);
		Key<Indices> getSimpleFormId(SimpleForm form);
		Key<VertexPool> getSimpleFormGeoPool();
		Key<IndexPool> getSimpleFormIdPool();
		Key<VertexPool> addVertexPool(uint8_t nbrAttributes, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t>const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent);
		Key<IndexPool> addIndexPool();
		Key<Vertices> addVertices(size_t nbrVertices, AGE::Vector<size_t> const &sizeBuffers, AGE::Vector<void *> const &buffers, Key<VertexPool> const &pool);
		Key<Indices> addIndices(size_t nbrIndices, AGE::Vector<uint32_t> const &buffers, Key<IndexPool> const &pool);
		GeometryManager &rmVertices(Key<VertexPool> const &vertexPool, Key<Vertices> &vertices);
		GeometryManager &rmIndices(Key<IndexPool> const &indexPool, Key<Indices> &indices);
		static void generateIcoSphere(size_t recursion, AGE::Vector<void *> &vertex, AGE::Vector<unsigned int> &index, size_t &nbrElementId, size_t &nbrElementGeo);
		GeometryManager &draw(GLenum mode, Key<Vertices> const &vertices, Key<VertexPool> const &pool);
		GeometryManager &draw(GLenum mode, Key<Indices> const &indices, Key<Vertices> const &vertices, Key<IndexPool> const &indexPool, Key<VertexPool> const &vertexPool);

	private:
		std::map<SimpleForm, Key<Vertices>> _simpleFormGeo;
		std::map<SimpleForm, Key<Indices>> _simpleFormId;
		Key<VertexPool> *_simpleFormPoolGeo;
		Key<IndexPool> *_simpleFormPoolId;
		void initSimpleForm();
		AGE::Vector<IndexPool *> _indexPool;
		AGE::Vector<VertexPool *> _vertexPool;
		VertexPool *getVertexPool(Key<VertexPool> const &key);
		IndexPool *getIndexPool(Key<IndexPool> const &key);
	};

}