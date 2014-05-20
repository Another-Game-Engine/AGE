#ifndef VERTEXMANAGER_HH_
# define VERTEXMANAGER_HH_

#define TEST_NEW_VERTEXMANAGER 1

#if TEST_NEW_VERTEXMANAGER

#include <Utils/OpenGL.hh>
#include <OpenGL/Key.hh>
#include <OpenGL/VertexBuffer.hh>
#include <Utils/Dependency.hpp>
#include <vector>
#include <utility>

namespace gl
{
	class Vertices;
	class VerticesPool;

	//!\file VerticesManager.hh
	//!\author Dorian Pinaud
	//!\version v1.0
	//!\class VerticesManager
	//!\brief Handle the geometry of the render
	class VerticesManager : public Dependency
	{
	public:
		// constructor
		VerticesManager();
		~VerticesManager();

		// handle pools
		Key<VerticesPool> const &addPool();
		Key<VerticesPool> const &addPool(uint8_t nbrAttributes, GLenum *typeComponent, uint8_t *sizeTypeComponent, uint8_t *nbrComponent);
		Key<VerticesPool> getPool(size_t index) const;
		size_t nbrPool() const;
		VerticesManager &rmPool(Key<VerticesPool> const &key);

		//handle Vertices
		Key<Vertices> const &addVertices(size_t nbrVertices, uint8_t nbrBuffers, size_t *sizeBuffers, void **buffers);
		VerticesManager &rmVertices(Key<Vertices> const &key);
		Key<Vertices> getVertices(size_t index) const;
		size_t getNbrVertices() const;

		// attach vertices to pools
		VerticesManager &attachVerticesToPool(Key<Vertices> const &vertices, Key<VerticesPool> const &pool);
		VerticesManager &detachVerticesToPool(Key<Vertices> const &vertices, Key<VerticesPool> const &pool);

	private:
		// data represent pools
		std::vector<std::pair<Key<VerticesPool>, VerticesPool>> _pools;
		std::vector<std::pair<Key<Vertices>, Vertices>> _vertices;
	};
}

#else
# include <queue>
# include <Utils/OpenGL.hh>
# include <Utils/Dependency.hpp>
# include <OpenGL/Vertice.hh>
# include <OpenGL/Attribute.hh>
# include <OpenGL/VertexArray.hh>
# include <OpenGL/VertexBuffer.hh>

template <uint8_t NBR_ATTRIBUTE> class VertexPool;
template <uint8_t NBR_ATTRIBUTE> class VertexPoolElement;

template <uint8_t NBR_ATTRIBUTE>
class VertexManager : public Dependency
{
public:
	VertexManager(std::array<Attribute, NBR_ATTRIBUTE> const &attributes);
	virtual ~VertexManager();
	VertexManager(VertexManager const &copy);
	VertexManager &operator=(VertexManager const &vertexmanager);
	bool init();
	void addVertice(Vertice<NBR_ATTRIBUTE> &vertice);
	void deleteVertice(Vertice<NBR_ATTRIBUTE> &vertice);
	void update();
	void callDraw(Vertice<NBR_ATTRIBUTE> const * const drawable, GLenum mode);

private:
	OpenGLTools::VertexArray _vertexArray;
	gl::VertexBuffer _indexBuffer;
	gl::VertexBuffer _dataBuffer;
	VertexPool<NBR_ATTRIBUTE> _pool;
	std::array<bool, NBR_ATTRIBUTE> _isAttributeActivate;
	std::array<Attribute, NBR_ATTRIBUTE> _attributes;
private:
	inline void sendVertexAttribPointerOnGPU();
	inline void sendMajorVertexDataOnGPU();
	inline void sendMinorVertexDataOnGPU();
};

enum StateVertexPool
{
	MINOR_UPDATE = 0,
	MAJOR_UPDATE,
	NO_UPDATE
};

template <uint8_t NBR_ATTRIBUTE>
class VertexPool
{
private:
	std::vector<VertexPoolElement<NBR_ATTRIBUTE>> _elements;
	std::size_t _sizeVertexBuffer;
	std::size_t _sizeIndicesBuffer;
	std::size_t _nbrVertex;
	std::size_t _nbrIndices;
	std::array<std::size_t, NBR_ATTRIBUTE> _pointerAttributes;
	std::queue<std::size_t> _updateBuffer;
	StateVertexPool _updateState;
	std::array<Attribute, NBR_ATTRIBUTE> _attributes;
public:
	VertexPool(std::array<Attribute, NBR_ATTRIBUTE> const &attributes);
	~VertexPool();
	VertexPool(VertexPool<NBR_ATTRIBUTE> const &copy);
	VertexPool &operator=(VertexPool const &vertexpool);

	std::size_t addElement(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteElement(Vertice<NBR_ATTRIBUTE> const &vertices);
	void fullClear();
	void clear();
	inline void computeOffset();

	VertexPoolElement<NBR_ATTRIBUTE> const &operator[](std::size_t index);
	size_t getNbrElement() const;
	std::size_t getSizeIndicesBuffer() const;
	std::size_t getSizeVertexBuffer() const;
	std::size_t getNbrVertex() const;
	std::size_t getNbrIndices() const;
	std::size_t getPointerAttribute(GLint index) const;
	StateVertexPool getUpdateState() const;
	bool getUpdateMinor(std::size_t &index);
	void resetState();
};

enum AddVerticesResult
{
	FAIL = 0,
	SET,
	ADD
};

template <uint8_t NBR_ATTRIBUTE>
class VertexPoolElement
{
public:
	VertexPoolElement(std::size_t nbrEntity, Vertice<NBR_ATTRIBUTE> const &vertices);
	VertexPoolElement(VertexPoolElement<NBR_ATTRIBUTE> const &copy);
	~VertexPoolElement();
	VertexPoolElement &operator=(VertexPoolElement<NBR_ATTRIBUTE> const &element);

	AddVerticesResult addVertice(Vertice<NBR_ATTRIBUTE> const &vertices);
	void deleteVertice();

	bool isEmpty() const;
	std::size_t getVertexOffset() const;
	std::size_t getIndicesOffset() const;
	std::size_t getByteOffset(uint8_t index) const;
	std::size_t getNbrByte(uint8_t index) const;
	Vertice<NBR_ATTRIBUTE> const &getVertex() const;

	inline void settingOffset(std::size_t vertexOffset, std::size_t indicesOffset, std::array<std::size_t, NBR_ATTRIBUTE> const &byteOffset, std::array<std::size_t, NBR_ATTRIBUTE> const &nbrByte);

private:
	std::size_t _nbrEntity;
	Vertice<NBR_ATTRIBUTE> _vertices;
	std::size_t _vertexOffset;
	std::size_t _indicesOffset;
	std::array<std::size_t, NBR_ATTRIBUTE> _byteOffset;
	std::array<std::size_t, NBR_ATTRIBUTE> _nbrByte;
};

# include "VertexManager.hpp"

#endif

#endif /*!VERTEXMANAGER_HH_*/
