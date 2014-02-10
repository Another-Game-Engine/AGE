
#include "EntityData.hh"
#include "Core/Engine.hh"
#include "Components/Component.hh"
#include "Entity.hh"
#include <limits>
#include <Core/AScene.hh>

EntityData::EntityData(AScene *scene) :
    PubSub(scene->getInstance<PubSub::Manager>()),
    _scene(scene),
	_flags(0),
	_localTranslation(0),
	_localRotation(0),
	_localScale(0),
	_globalTranslation(0),
	_globalRotation(0),
	_globalScale(0)
{
	removeFlags(ACTIVE);
}

EntityData::~EntityData()
{
	_components.clear();
}

Entity &EntityData::getHandle()
{
	return _handle;
}

AScene *EntityData::getScene() const
{
	return _scene;
}

void EntityData::setHandle(Entity &handle)
{
	_handle = handle;
}

glm::mat4 const  		&EntityData::getLocalTransform()
{
	return (_localTransform);
}

//  TO DELETE
glm::mat4   			&EntityData::setLocalTransform()
{
	_flags |= HAS_MOVED;
	return (_localTransform);
}

glm::mat4 const			&EntityData::getGlobalTransform() const
{
	return (_globalTransform);
}

// TO DELETE
void 					EntityData::computeGlobalTransform(glm::mat4 const &fatherTransform)
{
	_globalTransform = fatherTransform * _localTransform;
	_flags ^= HAS_MOVED;
}

void 					EntityData::computeGlobalTransform(const Entity &parent)
{

	_flags ^= HAS_MOVED;
}

void                    EntityData::translate(const glm::vec3 &v)
{
	_localTranslation += v;
	_flags |= HAS_MOVED;
}

void                    EntityData::setTranslation(const glm::vec3 &v)
{
	_localTranslation = v;
	_flags |= HAS_MOVED;
}

glm::vec3 const         &EntityData::getTranslation() const
{
	return _localTranslation;
}

void                    EntityData::rotate(const glm::vec3 &v)
{
	_localRotation += v;
	_flags |= HAS_MOVED;
}

void                    EntityData::setRotation(const glm::vec3 &v)
{
	_localRotation = v;
	_flags |= HAS_MOVED;
}

glm::vec3 const         &EntityData::getRotation() const
{
	return _localRotation;
}

void                    EntityData::scale(const glm::vec3 &v)
{
	_localScale += v;
	_flags |= HAS_MOVED;
}

void                    EntityData::setScale(const glm::vec3 &v)
{
	_localScale = v;
	_flags |= HAS_MOVED;
}

glm::vec3 const         &EntityData::getScale() const
{
	return _localScale;
}

size_t 					EntityData::getFlags() const
{
	return (_flags);
}

void 					EntityData::setFlags(size_t flags)
{
	_flags = flags;
}

void 					EntityData::addFlags(size_t flags)
{
	_flags |= flags;
}

void 					EntityData::removeFlags(size_t flags)
{
	flags &= _flags;
	_flags ^= flags;
}

Barcode                 &EntityData::getCode()
{
	return _code;
}

void                    EntityData::addTag(unsigned int tag)
{
	assert(tag < MAX_TAG_NUMBER, "Tags limit is 31");
	_code.add(tag);
}

void                    EntityData::removeTag(unsigned int tag)
{
	assert(tag < MAX_TAG_NUMBER, "Tags limit is 31");
	_code.remove(tag);
}
bool                    EntityData::isTagged(unsigned int tag) const
{
	assert(tag < MAX_TAG_NUMBER, "Tags limit is 31");
	return _code.isSet(tag);
}

bool                    EntityData::isTagged(Barcode &code)
{
	// TODO get the size of the code and assert if > 31
	return _code.match(code);
}

void EntityData::reset()
{
	_flags = 0;
	_localTranslation = glm::vec3(0);
	_localRotation = glm::vec3(0);
	_localScale = glm::vec3(0);
	_globalTranslation = glm::vec3(0);
	_globalRotation = glm::vec3(0);
	_globalScale = glm::vec3(0);
	_globalTransform = glm::mat4(1);
	_localTransform = glm::mat4(1);
	_code.reset();
	for (unsigned int i = 0; i < _components.size(); ++i)
	{
		unsigned int id = i + MAX_TAG_NUMBER;
		if (_components[i].get())
		{
			broadCast(std::string("componentRemoved" + std::to_string(id)), _handle);
			_components[i]->reset();
		}
		_components[i].reset();
	}
}

////////////////
//
// Graphnode

const Entity	    	&EntityData::getParent() const
{
	return _parent;
}

void 					EntityData::removeChild(Entity &child, bool notify)
{
	_childs.erase(child);
	if (notify)
	{
		child->removeParent(false);
	}
}

void 					EntityData::setParent(Entity &parent, bool notify)
{
	if (_parent.get())
	{
		_parent->removeChild(_handle, false);
	}
	if (notify)
		parent->addChild(_handle, false);
	if (!parent.get()) // if parent is null -> it's a root node
	{
		auto key = PubSubKey("graphNodeSetAsRoot");
		broadCast(key, _handle);
	}
	else if (!_parent.get()) // if it was a root node
	{
		auto key = PubSubKey("graphNodeNotARoot");
		broadCast(key, _handle);
	}
	_parent = parent;
}

void 					EntityData::addChild(Entity &child, bool notify)
{
	_childs.insert(child);
	if (notify)
	{
		child->setParent(_handle, false);
	}
}

void                    EntityData::removeParent(bool notify)
{
	if (notify && _parent.get())
	{
		_parent->removeChild(_handle);
	}
	auto key = PubSubKey("graphNodeSetAsRoot");
	broadCast(key, _handle);
	_parent = Entity(std::numeric_limits<unsigned int>::max(), nullptr);
}

std::set<Entity>::iterator EntityData::getChildsBegin()
{
	return std::begin(_childs);
}
		
std::set<Entity>::iterator EntityData::getChildsEnd()
{
	return std::end(_childs);
}

//
//
//////////////