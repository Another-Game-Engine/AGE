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
	}

	void GeometryManager::generateIcoSphere(size_t recursion, glm::vec3 **vertex, glm::u32vec3 **indices, size_t &nbrElementId, size_t &nbrElementGeo)
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

		*vertex = new glm::vec3[vertexTab.size()];
		*indices = new glm::u32vec3[idTab.size()];
		//*vertex = (glm::vec3 *)quadForm;
		//*indices = (glm::u32vec3 *)quadFormId;
		memcpy(*vertex, vertexTab.data(), vertexTab.size() * sizeof(glm::vec3));
		memcpy(*indices, idTab.data(), idTab.size() * sizeof(glm::u32vec3));
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
		size_t nbrVertices = 4;
		size_t nbrElement = 6;
		uint8_t const nbrBuffer = 1;
		size_t sizeBuffer = 4 * 3 * nbrVertices;
		void *buffer[nbrBuffer];
		buffer[0] = (void *)quadForm;
		_simpleFormGeo[SimpleForm::QUAD] = addVertices(nbrVertices, nbrBuffer, &sizeBuffer, buffer);
		_simpleFormId[SimpleForm::QUAD] = addIndices(nbrElement, quadFormId);
		attachVerticesToVertexPool(_simpleFormGeo[SimpleForm::QUAD], *_simpleFormPoolGeo);
		attachIndicesToIndexPool(_simpleFormId[SimpleForm::QUAD], *_simpleFormPoolId);
		return (*this);
	}

	GeometryManager &GeometryManager::createSphereSimpleForm()
	{
		auto &element = _simpleFormGeo.find(SimpleForm::SPHERE);
		if (element != _simpleFormGeo.end())
			return (*this);
		initSimpleForm();
		size_t const nbrBuffer = 1;
		void *buffer[nbrBuffer];
		uint32_t *id;
		size_t nbrElementGeo;
		size_t nbrElementId;
		generateIcoSphere(1, (glm::vec3 **)&buffer[0], (glm::u32vec3 **)&id, nbrElementId, nbrElementGeo);
		size_t sizeBuffer = 4 * 3 * nbrElementGeo;
		_simpleFormGeo[SimpleForm::SPHERE] = addVertices(nbrElementGeo, nbrBuffer, &sizeBuffer, buffer);
		_simpleFormId[SimpleForm::SPHERE] = addIndices(nbrElementId, id);
		attachVerticesToVertexPool(_simpleFormGeo[SimpleForm::SPHERE], *_simpleFormPoolGeo);
		attachIndicesToIndexPool(_simpleFormId[SimpleForm::SPHERE], *_simpleFormPoolId);
		delete[] buffer[0];
		delete[] id;
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

	Key<VertexPool> GeometryManager::addVertexPool()
	{
		Key<VertexPool> key = Key<VertexPool>::createKey();
		_vertexPool[key] = VertexPool();
		return (key);
	}

	Key<IndexPool> GeometryManager::addIndexPool()
	{
		Key<IndexPool> key = Key<IndexPool>::createKey();
		_indexPool[key];
		return (key);
	}

	Key<VertexPool> GeometryManager::addVertexPool(uint8_t nbrAttributes, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent)
	{
		Key<VertexPool> key = Key<VertexPool>::createKey();

		_vertexPool[key] = VertexPool(nbrAttributes, typeComponent, sizeTypeComponent, nbrComponent);
		return (key);
	}

	Key<VertexPool> GeometryManager::getVertexPool(size_t target) const
	{
		if (target >= _vertexPool.size())
			return (Key<VertexPool>());
		auto &element = _vertexPool.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	Key<IndexPool> GeometryManager::getIndexPool(size_t target) const
	{
		if (target >= _vertexPool.size())
			return (Key<IndexPool>());
		auto &element = _indexPool.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	size_t GeometryManager::nbrVertexPool() const
	{
		return (_vertexPool.size());
	}

	size_t GeometryManager::nbrIndexPool() const
	{
		return (_indexPool.size());
	}

	// the key will be set to empty
	GeometryManager &GeometryManager::rmVertexPool(Key<VertexPool> &key)
	{
		if (getVertexPool(key) == NULL)
			return (*this);
		_vertexPool.erase(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndexPool(Key<IndexPool> &key)
	{
		if (getIndexPool(key) == NULL)
			return (*this);
		_indexPool.erase(key);
		key.destroy();
		return (*this);
	}

	Key<Vertices> GeometryManager::getVertices(size_t target) const
	{
		if (target >= _vertices.size())
			return (Key<Vertices>());
		auto &element = _vertices.begin();
		for (size_t index = 0; index < target; ++index)
			++element;
		return (element->first);
	}

	size_t GeometryManager::getNbrVertices() const
	{
		return (_vertices.size());
	}

	Key<Vertices> GeometryManager::addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers)
	{
		Key<Vertices> key = Key<Vertices>::createKey();

		_vertices[key] = Vertices(nbrVertices, nbrBuffers, sizeBuffers, buffers);
		return (key);
	}

	Key<Indices> GeometryManager::addIndices(size_t nbrIndices, uint32_t *buffer)
	{
		Key<Indices> key = Key<Indices>::createKey();

		_indices[key] = Indices(nbrIndices, buffer);
		return (key);
	}

	GeometryManager &GeometryManager::rmVertices(Key<Vertices> &key)
	{
		if (getVertices(key) == NULL)
			return (*this);
		dettachVerticesToVertexPool(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::rmIndices(Key<Indices> &key)
	{
		if (getIndices(key) == NULL)
			return (*this);
		dettachIndicesToIndexPool(key);
		key.destroy();
		return (*this);
	}

	GeometryManager &GeometryManager::attachVerticesToVertexPool(Key<Vertices> const &keyVertices, Key<VertexPool> const &keyPool)
	{
		Attach<Vertices, VertexPool> newAttach(getVertexPool(keyPool));
		
		dettachVerticesToVertexPool(keyVertices);
		newAttach.element = newAttach.pool->addElementPool(*getVertices(keyVertices));
		_vertexAttach[keyVertices] = newAttach;
		return (*this);
	}

	GeometryManager &GeometryManager::dettachVerticesToVertexPool(Key<Vertices> const &key)
	{
		Attach<Vertices, VertexPool> *attach = getVertexAttach(key);
		if (attach == NULL)
			return (*this);
		attach->pool->rmElement(attach->element);
		_vertexAttach.erase(key);
		return (*this);
	}

	GeometryManager &GeometryManager::attachIndicesToIndexPool(Key<Indices> const &keyIndices, Key<IndexPool> const &keyPool)
	{
		Attach<Indices, IndexPool> newAttach(getIndexPool(keyPool));
		dettachIndicesToIndexPool(keyIndices);
		newAttach.element = newAttach.pool->addElementPool(*getIndices(keyIndices));
		_indexAttach[keyIndices] =  newAttach;
		return (*this);
	}

	GeometryManager &GeometryManager::dettachIndicesToIndexPool(Key<Indices> const &key)
	{
		Attach<Indices, IndexPool> *attach;
		if ((attach = getIndexAttach(key)) == NULL)
			return (*this);
		attach->pool->rmElement(attach->element);
		_indexAttach.erase(key);
		return (*this);
	}

	VertexPool *GeometryManager::getVertexPool(Key<VertexPool> const &key)
	{
		if (!key)
			assert(0);
		if (_vertexPool.size() == 0)
			assert(0);
		if (_optimizerVertexPoolSearch.first == key)
			return (_optimizerVertexPoolSearch.second);
		auto &vertexPool = _vertexPool.find(key);
		if (vertexPool == _vertexPool.end())
			assert(0);
		_optimizerVertexPoolSearch.first = key;
		_optimizerVertexPoolSearch.second = &vertexPool->second;
		return (&vertexPool->second);
	}

	IndexPool *GeometryManager::getIndexPool(Key<IndexPool> const &key)
	{
		if (!key)
			assert(0);
		if (_indexPool.size() == 0)
			assert(0);
		if (_optimizerIndexPoolSearch.first == key)
			return (_optimizerIndexPoolSearch.second);
		auto &indexPool = _indexPool.find(key);
		if (indexPool == _indexPool.end())
			assert(0);
		_optimizerIndexPoolSearch.first = key;
		_optimizerIndexPoolSearch.second = &indexPool->second;
		return (&indexPool->second);
	}

	Indices *GeometryManager::getIndices(Key<Indices> const &key)
	{
		if (!key)
			assert(0);
		if (_indices.size() == 0)
			assert(0);
		if (_optimizerIndicesSearch.first == key)
			return (_optimizerIndicesSearch.second);
		auto &indices = _indices.find(key);
		if (indices == _indices.end())
			assert(0);
		_optimizerIndicesSearch.first = key;
		_optimizerIndicesSearch.second = &indices->second;
		return (&indices->second);
	}

	Vertices *GeometryManager::getVertices(Key<Vertices> const &key)
	{
		if (!key)
			assert(0);
		if (_vertices.size() == 0)
			assert(0);
		if (_optimizerVerticesSearch.first == key)
			return (_optimizerVerticesSearch.second);
		auto &vertices = _vertices.find(key);
		if (vertices == _vertices.end())
			assert(0);
		_optimizerVerticesSearch.first = key;
		_optimizerVerticesSearch.second = &vertices->second;
		return (&vertices->second);
	}

	Attach<Vertices, VertexPool> *GeometryManager::getVertexAttach(Key<Vertices> const &key)
	{
		if (!key)
			assert(0);
		if (_optimizerVertexAttachSearch.first == key)
			return (_optimizerVertexAttachSearch.second);
		if (_vertexAttach.size() == 0)
			return (NULL);
		auto &verticesAttach = _vertexAttach.find(key);
		if (verticesAttach == _vertexAttach.end())
			return (NULL);
		_optimizerVertexAttachSearch.first = key;
		_optimizerVertexAttachSearch.second = &verticesAttach->second;
		return (&verticesAttach->second);
	}

	Attach<Indices, IndexPool> *GeometryManager::getIndexAttach(Key<Indices> const &key)
	{
		if (!key)
			assert(0);
		if (_indexAttach.size() == 0)
			return (NULL);
		if (_optimizerIndexAttachSearch.first == key)
			return (_optimizerIndexAttachSearch.second);
		auto &indexAttach = _indexAttach.find(key);
		if (indexAttach == _indexAttach.end())
			return (NULL);
		_optimizerIndexAttachSearch.first = key;
		_optimizerIndexAttachSearch.second = &indexAttach->second;
		return (&indexAttach->second);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Vertices> const &vertices)
	{
		auto attachment = getVertexAttach(vertices);
		attachment->pool->startContext();
		attachment->pool->bind();
		attachment->pool->syncronisation();
		attachment->pool->draw(mode, attachment->element);
		attachment->pool->endContext();
		return (*this);
	}

	GeometryManager &GeometryManager::draw(GLenum mode, Key<Indices> const &indices, Key<Vertices> const &vertices)
	{
		auto attachmentVertices = getVertexAttach(vertices);
		auto attachmentIndices = getIndexAttach(indices);
		attachmentVertices->pool->startContext(*attachmentIndices->pool);
		attachmentVertices->pool->syncronisation();
		attachmentIndices->pool->syncronisation();
		attachmentVertices->pool->draw(mode, attachmentIndices->element, attachmentVertices->element);
		attachmentVertices->pool->endContext();
		return (*this);
	}
}