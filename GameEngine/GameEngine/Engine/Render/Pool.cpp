#include <Render/Pool.hh>

#define INDEXPOOL 1

namespace gl
{
	AttributeData::AttributeData()
		: typeComponent(GL_FLOAT),
		sizeTypeComponent(sizeof(float)),
		nbrComponent(4),
		sizeAttribute(0),
		offsetAttribute(0)
	{

	}

	AttributeData::AttributeData(GLenum tc, uint8_t stc, uint8_t nbrC)
		: typeComponent(tc),
		sizeTypeComponent(stc),
		nbrComponent(nbrC),
		sizeAttribute(0),
		offsetAttribute(0)
	{

	}

	AttributeData::~AttributeData()
	{

	}

	AttributeData::AttributeData(AttributeData const &copy)
		: typeComponent(copy.typeComponent),
		sizeTypeComponent(copy.sizeTypeComponent),
		nbrComponent(copy.nbrComponent),
		sizeAttribute(copy.sizeAttribute),
		offsetAttribute(copy.offsetAttribute)
	{

	}

	AttributeData &AttributeData::operator=(AttributeData const &a)
	{
		typeComponent = a.typeComponent;
		sizeTypeComponent = a.sizeTypeComponent;
		nbrComponent = a.nbrComponent;
		sizeAttribute = a.sizeAttribute;
		offsetAttribute = a.offsetAttribute;
		return (*this);
	}

	VertexPool::VertexPool()
		: Pool<VertexPool, Vertices, VertexBuffer>(),
		_indexPool(NULL),
		_contextSet(false)
	{
	}

	VertexPool::VertexPool(uint8_t nbrAttribute, AGE::Vector<GLenum> const &typeComponent, AGE::Vector<uint8_t> const &sizeTypeComponent, AGE::Vector<uint8_t> const &nbrComponent)
		: Pool<VertexPool, Vertices, VertexBuffer>(nbrAttribute, typeComponent, sizeTypeComponent, nbrComponent),
		_indexPool(NULL),
		_contextSet(false)
	{
	}

	VertexPool::VertexPool(VertexPool const &copy)
		: Pool<VertexPool, Vertices, VertexBuffer>(copy),
		_indexPool(copy._indexPool),
		_contextSet(copy._contextSet)
	{
	}

	VertexPool &VertexPool::operator=(VertexPool const &v)
	{
		Pool<VertexPool, Vertices, VertexBuffer>::operator=(v);
		_indexPool = v._indexPool;
		_contextSet = v._contextSet;
		return (*this);
	}

	VertexPool::~VertexPool()
	{
	}

	VertexPool const &VertexPool::bind() const
	{
		_buffer.bind();
		for (size_t index = 0; index < _attributes.size(); ++index)
		{
			auto const &attribute = _attributes[index];
			_buffer.attribute(index, attribute.nbrComponent, attribute.typeComponent, attribute.offsetAttribute);
		}
		return (*this);
	}

	VertexPool &VertexPool::startContext(IndexPool &i)
	{
		_context.bind();
		if (_indexPool != &i)
			_setContext = false;
		_indexPool = &i;
		if (!_setContext)
		{
			bind();
			i.bind();
			_contextSet = true;
		}
		return (*this);
	}

	VertexPool &VertexPool::startContext()
	{
		_context.bind();
		if (!_setContext)
		{
			bind();
			_contextSet = true;
		}
		return (*this);
	}

	VertexPool const &VertexPool::endContext() const
	{
		_context.unbind();
		return (*this);
	}

	VertexPool &VertexPool::draw(GLenum mode, Key<Element<Vertices>> const &key)
	{
		auto element = getElementPool(key);
		auto const &memory = _poolMemory[element->memoryIndex];
		glDrawArrays(mode, GLsizei(memory.getElementStart()), GLint(memory.getNbrElement()));
		return (*this);
	}

	VertexPool &VertexPool::draw(GLenum mode, Key<Element<Indices>> const &i, Key<Element<Vertices>> const &v)
	{
		auto element = getElementPool(v);
		auto const &memory = _poolMemory[element->memoryIndex];
		_indexPool->draw(mode, i, memory.getElementStart());
		return (*this);
	}

	IndexPool::IndexPool()
		: Pool<IndexPool, Indices, IndexBuffer>(1, {GL_UNSIGNED_INT}, {sizeof(unsigned int)}, {1})
	{
	}

	IndexPool::~IndexPool()
	{
	}

	IndexPool::IndexPool(IndexPool const &copy)
		: Pool<IndexPool, Indices, IndexBuffer>(copy)
	{

	}

	IndexPool &IndexPool::operator=(IndexPool const &i)
	{
		Pool<IndexPool, Indices, IndexBuffer>::operator=(i);
		return (*this);
	}

	IndexPool &IndexPool::draw(GLenum mode, Key<Element<Indices>> const &i, size_t start)
	{
		auto element = getElementPool(i);
		auto const &memory = _poolMemory[element->memoryIndex];
		glDrawElementsBaseVertex(mode, GLsizei(memory.getNbrElement()), GL_UNSIGNED_INT, (const GLvoid *)(memory.getElementStart() * sizeof(unsigned int)), (GLint)start);
		return (*this);
	}

	IndexPool const &IndexPool::bind() const
	{
		_buffer.bind();
		return (*this);
	}
}