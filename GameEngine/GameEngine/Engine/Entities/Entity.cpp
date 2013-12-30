
#include "Entity.hh"
#include "Core/Engine.hh"
#include "Components/Component.hh"
#include "Handle.hh"
#include <limits>

Entity::Entity(AScene *scene) :
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
}

Entity::~Entity()
{
	_components.clear();
}

Handle &Entity::getHandle()
{
	return _handle;
}

void Entity::setHandle(Handle &handle)
{
	_handle = handle;
}

glm::mat4 const  		&Entity::getLocalTransform()
{
	return (_localTransform);
}

//  TO DELETE
glm::mat4   			&Entity::setLocalTransform()
{
	_flags |= HAS_MOVED;
	return (_localTransform);
}

glm::mat4 const			&Entity::getGlobalTransform() const
{
	return (_globalTransform);
}

// TO DELETE
void 					Entity::computeGlobalTransform(glm::mat4 const &fatherTransform)
{
	_globalTransform = fatherTransform * _localTransform;
	_flags ^= HAS_MOVED;
}

void 					Entity::computeGlobalTransform(const Handle &parent)
{

	_flags ^= HAS_MOVED;
}

void                    Entity::translate(const glm::vec3 &v)
{
	_localTranslation += v;
	_flags |= HAS_MOVED;
}

void                    Entity::setTranslation(const glm::vec3 &v)
{
	_localTranslation = v;
	_flags |= HAS_MOVED;
}

glm::vec3 const         &Entity::getTranslation() const
{
	return _localTranslation;
}

void                    Entity::rotate(const glm::vec3 &v)
{
	_localRotation += v;
	_flags |= HAS_MOVED;
}

void                    Entity::setRotation(const glm::vec3 &v)
{
	_localRotation = v;
	_flags |= HAS_MOVED;
}

glm::vec3 const         &Entity::getRotation() const
{
	return _localRotation;
}

void                    Entity::scale(const glm::vec3 &v)
{
	_localScale += v;
	_flags |= HAS_MOVED;
}

void                    Entity::setScale(const glm::vec3 &v)
{
	_localScale = v;
	_flags |= HAS_MOVED;
}

glm::vec3 const         &Entity::getScale() const
{
	return _localScale;
}

size_t 					Entity::getFlags() const
{
	return (_flags);
}

void 					Entity::setFlags(size_t flags)
{
	_flags = flags;
}

void 					Entity::addFlags(size_t flags)
{
	_flags |= flags;
}

void 					Entity::removeFlags(size_t flags)
{
	flags &= _flags;
	_flags ^= flags;
}

Barcode                       &Entity::getCode()
{
	return _code;
}

bool Entity::hasComponent(unsigned int componentId) const
{
		return _code.isSet(componentId);
}