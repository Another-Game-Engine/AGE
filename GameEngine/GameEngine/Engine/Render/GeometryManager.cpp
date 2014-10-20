#include <Render/GeometryManager.hh>
#include <Render/Data.hh>
#include <Render/Pool.hh>
#include <iostream>
#include <string>
#include <cassert>
#include <unordered_map>
#include <Render/SimpleFormGeometry.hh>
#include <glm/glm.hpp>

typedef std::pair<uint32_t, uint32_t>				idxPair_t;
typedef std::map<idxPair_t, uint32_t>				idxHash_t;

static uint32_t getMiddlePoint(std::vector<glm::vec3> &vertexTab, idxHash_t &middlePoints, uint32_t p1, uint32_t p2)
{
	// first check if we have it already
	bool firstIsSmaller = p1 < p2;
	uint32_t smallerIndex = firstIsSmaller ? p1 : p2;
	uint32_t greaterIndex = firstIsSmaller ? p2 : p1;
	idxPair_t key;

	key.first = smallerIndex;
	key.second = greaterIndex;

	uint32_t ret;

	idxHash_t::iterator it = middlePoints.find(key);
	if (it != middlePoints.end())
	{
		return it->second;
	}

	// not in cache, calculate it
	glm::vec3 point1 = vertexTab[p1];
	glm::vec3 point2 = vertexTab[p2];
	glm::vec3 middle = (point1 + point2) / 2.0f;

	// add vertex makes sure point is on unit sphere
	vertexTab.push_back(glm::normalize(glm::vec3(middle)));

	ret = uint32_t(vertexTab.size() - 1);

	// store it, return index
	middlePoints[key] = ret;
	return ret;
}

namespace gl
{
	GeometryManager::GeometryManager()
		: _simpleFormPoolGeo(NULL),
		_simpleFormPoolId(NULL)
	{
	}

	GeometryManager::~GeometryManager()
	{
		if (_simpleFormPoolGeo != NULL)
			delete _simpleFormPoolGeo;
		if (_simpleFormPoolId != NULL)
			delete _simpleFormPoolId;
		for (auto &pool : _indexPool)
		{
			if (pool != NULL)
				delete pool;
		}
		for (auto &pool : _vertexPool)
		{
			if (pool != NULL)
				delete pool;
		}
	}

	void GeometryManager::generateIcoSphere(size_t recursion, AGE::Vector<void *> &vertex, AGE::Vector<unsigned int> &indices, size_t &nbrElementId, size_t &nbrElementGeo)
	{
		idxHash_t					middlePoints;
		AGE::Vector<glm::vec3>		vertexTab;
		AGE::Vector<glm::u32vec3>	idTab;
		uint32_t					currentIdx = 0;

		// create 12 vertices of a icosahedron
		float t = (1.0f + std::sqrt(5.0f)) / 2.0f;

		vertexTab.push_back(glm::normalize(glm::vec3(-1, t, 0)));
		vertexTab.push_back(glm::normalize(glm::vec3(1, t, 0)));
		vertexTab.push_back(glm::normalize(glm::vec3(-1, -t, 0)));
		vertexTab.push_back(glm::normalize(glm::vec3(1, -t, 0)));

		vertexTab.push_back(glm::normalize(glm::vec3(0, -1, t)));
		vertexTab.push_back(glm::normalize(glm::vec3(0, 1, t)));
		vertexTab.push_back(glm::normalize(glm::vec3(0, -1, -t)));
		vertexTab.push_back(glm::normalize(glm::vec3(0, 1, -t)));

		vertexTab.push_back(glm::normalize(glm::vec3(t, 0, -1)));
		vertexTab.push_back(glm::normalize(glm::vec3(t, 0, 1)));
		vertexTab.push_back(glm::normalize(glm::vec3(-t, 0, -1)));
		vertexTab.push_back(glm::normalize(glm::vec3(-t, 0, 1)));

		idTab.push_back(glm::u32vec3(0, 11, 5));
		idTab.push_back(glm::u32vec3(0, 5, 1));
		idTab.push_back(glm::u32vec3(0, 1, 7));
		idTab.push_back(glm::u32vec3(0, 7, 10));
		idTab.push_back(glm::u32vec3(0, 10, 11));

		// 5 adjacent faces 
		idTab.push_back(glm::u32vec3(1, 5, 9));
		idTab.push_back(glm::u32vec3(5, 11, 4));
		idTab.push_back(glm::u32vec3(11, 10, 2));
		idTab.push_back(glm::u32vec3(10, 7, 6));
		idTab.push_back(glm::u32vec3(7, 1, 8));

		// 5 faces around point 3
		idTab.push_back(glm::u32vec3(3, 9, 4));
		idTab.push_back(glm::u32vec3(3, 4, 2));
		idTab.push_back(glm::u32vec3(3, 2, 6));
		idTab.push_back(glm::u32vec3(3, 6, 8));
		idTab.push_back(glm::u32vec3(3, 8, 9));

		// 5 adjacent faces 
		idTab.push_back(glm::u32vec3(4, 9, 5));
		idTab.push_back(glm::u32vec3(2, 4, 11));
		idTab.push_back(glm::u32vec3(6, 2, 10));
		idTab.push_back(glm::u32vec3(8, 6, 7));
		idTab.push_back(glm::u32vec3(9, 8, 1));

		// refine triangles
		for (int i = 0; i < recursion; i++)
		{
			std::vector<glm::u32vec3>			idTab2;

			for (auto indices : idTab)
			{
				// replace triangle by 4 triangles
				uint32_t a = getMiddlePoint(vertexTab, middlePoints, indices.x, indices.y);
				uint32_t b = getMiddlePoint(vertexTab, middlePoints, indices.y, indices.z);
				uint32_t c = getMiddlePoint(vertexTab, middlePoints, indices.z, indices.x);

				idTab2.push_back(glm::u32vec3(indices.x, a, c));
				idTab2.push_back(glm::u32vec3(indices.y, b, a));
				idTab2.push_back(glm::u32vec3(indices.z, c, b));
				idTab2.push_back(glm::u32vec3(a, b, c));
			}
			idTab = idTab2;
		}
		vertex[0] = new uint8_t[vertexTab.size() * sizeof(glm::vec3)];
		for (size_t index = 0; index < vertexTab.size(); ++index)
		{
			((float *)vertex[0])[index] = vertexTab[index].x;
			((float *)vertex[0])[index + 1] = vertexTab[index].y;
			((float *)vertex[0])[index + 2] = vertexTab[index].z;
		}
		indices.resize(idTab.size() * 3);
		for (size_t index = 0; index < idTab.size(); ++index)
		{
			indices[index] = idTab[0].x;
			indices[index + 1] = idTab[1].y;
			indices[index + 2] = idTab[2].z;
		}
		nbrElementGeo = vertexTab.size();
		nbrElementId = idTab.size();
	}

	void GeometryManager::initSimpleForm()
	{
		if (_simpleFormPoolGeo == NULL)
		{
			_simpleFormPoolGeo = new Key<VertexPool>(addVertexPool(1, { GL_FLOAT }, { sizeof(float) }, {3}));
		}
		if (_simpleFormPoolId == NULL)
			_simpleFormPoolId = new Key<IndexPool>(addIndexPool());
	}

	GeometryManager &GeometryManager::createQuadSimpleForm()
	{
		auto &element = _simpleFormGeo.find(SimpleForm::QUAD);
		if (element != _simpleFormGeo.end())
			return (*this);
		initSimpleForm();
		_simpleFormGeo[SimpleForm::QUAD] = addVertices(4, { 4 * 3 * 4 }, {&quadForm[0]}, *_simpleFormPoolGeo);
		_simpleFormId[SimpleForm::QUAD] = addIndices(6, quadFormId, *_simpleFormPoolId);
		return (*this);
	}

	GeometryManager &GeometryManager::createSphereSimpleForm()
	{
		auto &element = _simpleFormGeo.find(SimpleForm::SPHERE);
		if (element != _simpleFormGeo.end())
			return (*this);
		initSimpleForm();
		size_t const nbrBuffer = 1;
		std::vector<void *> buffer = {NULL};
		std::vector<unsigned int> id;
		size_t nbrElementGeo;
		size_t nbrElementId;
//<<<<<<< HEAD
//		generateIcoSphere(3, (glm::vec3 **)&buffer[0], (glm::u32vec3 **)&id, nbrElementId, nbrElementGeo);
//=======
		generateIcoSphere(1, buffer, id, nbrElementId, nbrElementGeo);
//>>>>>>> 6a474651760b3a141c2d6ae2b76f4559f089f055
		size_t sizeBuffer = 4 * 3 * nbrElementGeo;
		_simpleFormGeo[SimpleForm::SPHERE] = addVertices(nbrElementGeo, { 4 * 3 * nbrElementGeo }, buffer, *_simpleFormPoolGeo);
		_simpleFormId[SimpleForm::SPHERE] = addIndices(nbrElementId, id, *_simpleFormPoolId);
		if (buffer[0] != NULL)
			delete buffer[0];
		return (*this);
	}


	Key<Vertices> GeometryManager::getSimpleFormGeo(SimpleForm form)
	{
		auto key = _simpleFormGeo.find(form);
		if (key == _simpleFormGeo.end())
			assert(0);
		return (key->second);
	}

	Key<Indices> GeometryManager::getSimpleFormId(SimpleForm form)
	{
		auto key = _simpleFormId.find(form);
		if (key == _simpleFormId.end())
			assert(0);
		return (key->second);
	}
	
	Key<VertexPool> GeometryManager::getSimpleFormGeoPool()
	{
		return (*_simpleFormPoolGeo);
	}

	Key<IndexPool> GeometryManager::getSimpleFormIdPool()
	{
		return (*_simpleFormPoolId);
	}

	Key<IndexPool> GeometryManager::addIndexPool()
	{
		Key<IndexPool> key = Key<IndexPool>::createKey();
		if (key.getId() >= _indexPool.size())
			_indexPool.push_back(NULL);
		_indexPool[key.getId()] = new IndexPool();
		return (key);
	}

	Key<VertexPool> GeometryManager::addVertexPool(uint8_t nbrAttributes, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent)
	{
		Key<VertexPool> key = Key<VertexPool>::createKey();
		if (key.getId() >= _vertexPool.size())
			_vertexPool.push_back(NULL);
		_vertexPool[key.getId()] = new VertexPool(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (key);
	}

	Key<Vertices> GeometryManager::addVertices(size_t nbrVertices, AGE::Vector<size_t> const &sizeBuffers, AGE::Vector<void *> const &buffers, Key<VertexPool> const &pool)
	{
		auto p = getVertexPool(pool);
		return (p->addElementPool(Data(nbrVertices, sizeBuffers, buffers)));
	}

	Key<Indices> GeometryManager::addIndices(size_t nbrIndices, AGE::Vector<uint32_t> const &buffer, Key<IndexPool> const &pool)
	{
		auto p = getIndexPool(pool);
		return (p->addElementPool(Data(nbrIndices, buffer)));
	}

	GeometryManager &GeometryManager::rmVertices(Key<VertexPool> const &vertexPool, Key<Vertices> &vertices)
	{
		auto v = getVertexPool(vertexPool);
		v->rmElement(vertices);
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndices(Key<IndexPool> const &indexPool, Key<Indices> &indices)
	{
		auto i = getIndexPool(indexPool);
		i->rmElement(indices);
		return (*this);
	}

	VertexPool *GeometryManager::getVertexPool(Key<VertexPool> const &key)
	{
		if (!key)
			assert(0);
		if (_vertexPool.size() == 0)
			assert(0);
		return (_vertexPool[key.getId()]);
	}

	IndexPool *GeometryManager::getIndexPool(Key<IndexPool> const &key)
	{
		if (!key)
			assert(0);
		if (_indexPool.size() == 0)
			assert(0);
		return (_indexPool[key.getId()]);
	}


	GeometryManager &GeometryManager::draw(GLenum mode, Key<Vertices> const &vertices, Key<VertexPool> const &pool)
	{
		auto p = getVertexPool(pool);
		p->startContext();
		p->bind();
		p->syncronisation();
		p->draw(mode, vertices);
		p->endContext();
		return (*this);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Indices> const &indices, Key<Vertices> const &vertices, Key<IndexPool> const &indexPool, Key<VertexPool> const &vertexPool)
	{
		auto pv = getVertexPool(vertexPool);
		auto pi = getIndexPool(indexPool);
		pv->startContext(*pi);
		pv->syncronisation();
		pi->syncronisation();
		pv->draw(mode, indices, vertices);
		pv->endContext();
		return (*this);
	}
}