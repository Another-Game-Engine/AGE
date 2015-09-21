#include "Link.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Utils/Debug.hpp>
#include <Utils/MatrixConversion.hpp>
#include <Core/AScene.hh>
#include <ComponentsCore/Collider.hpp>
#include <BFC/BFCLinkTracker.hpp>


using namespace AGE;

#ifdef AGE_BFC
void Link::BFC_ADD()
{
	registerToTracker();
}

void Link::BFC_REMOVE()
{
	unregisterFromTracker();
}
#endif

bool Link::hasChildren() const { return _children.size() > 0; };
bool Link::hasParent() const { return _parent != nullptr && _parent != _scene->getRootLink(); }
bool Link::hasParent(const Link *parent) const { return _parent == parent; }
Link *Link::getParent() { return _parent; }
const std::vector<Link*> &Link::getChildren() const { return _children; }

void Link::setPosition(const glm::vec3 &v, bool recalculate)
{
	_userModification = true;
	internalSetPosition(v, recalculate);
}

void Link::setScale(const glm::vec3 &v, bool recalculate)
{
	const glm::vec3 oldScale = getScale();
	_userModification = true;
	internalSetScale(v, recalculate);
	Collider *collider = static_cast<Collider *>(_entityPtr->getComponent(Component<Collider>::getTypeId()));
	if (collider != nullptr)
	{
		collider->scale(getScale() / oldScale);
	}
}

void Link::setScale(float v, bool recalculate)
{
	setScale(glm::vec3(v, v, v), recalculate);
}


void Link::setOrientation(const glm::quat &v, bool recalculate)
{
	_userModification = true;
	internalSetOrientation(v, recalculate);
}

void Link::setForward(const glm::vec3 &v, bool recalculate)
{
	_userModification = true;
	internalSetForward(v, recalculate);
}

void Link::setTransform(const glm::mat4 &t, bool recalculate)
{
	_userModification = true;
	internalSetTransform(t, recalculate);
}


void Link::internalSetPosition(const glm::vec3 &v, bool recalculate)
{
    BFC_ADD();
	_localDirty = true;
	_position = v;
	if (recalculate)
	{
		_updateGlobalTransform();
	}
}

void Link::internalSetForward(const glm::vec3 &v, bool recalculate)
{
	BFC_ADD();
	_localDirty = true;
	glm::vec4 get = glm::mat4(glm::toMat4(_orientation) * glm::translate(glm::mat4(1), v))[3];
	_position.x = _position.x + get.x;
	_position.y = _position.y + get.y;
	_position.z = _position.z + get.z;
	if (recalculate)
	{
		_updateGlobalTransform();
	}
}

void Link::internalSetTransform(const glm::mat4 &t, bool recalculate)
{
	BFC_ADD();
	//_localDirty = true;
	//auto p = t * glm::vec4(1,1,1,1);
	//_position = glm::vec3(p.x, p.y, p.z);
	//_scale = scaleFromMat4(t);
	//_localTransformation = p;
	//auto rot = rotFromMat4(t, true);
	//_orientation = glm::quat(rot);
	//_position = posFromMat4(t);
	//_localTransformation = glm::translate(t, _position);
	_localTransformation = t;
	if (recalculate)
	{
		_updateGlobalTransform();
	}
}


void Link::internalSetScale(const glm::vec3 &v, bool recalculate)
{
	BFC_ADD();
	_localDirty = true;
	_scale = v;
	if (recalculate)
	{
		_updateGlobalTransform();
	}
}

void Link::internalSetOrientation(const glm::quat &v, bool recalculate)
{
	BFC_ADD();
	_localDirty = true;
	_orientation = v;
	if (recalculate)
	{
		_updateGlobalTransform();
	}
}

const glm::mat4 Link::getLocalTransform() const
{
	return const_cast<Link *>(this)->getLocalTransform();
}

const glm::mat4 &Link::getLocalTransform()
{
	if (_localDirty)
	{
		_localTransformation = glm::mat4(1);
		_localTransformation = glm::translate(_localTransformation, _position);
		_localTransformation = _localTransformation * glm::toMat4(_orientation);
		_localTransformation = glm::scale(_localTransformation, _scale);
		_localDirty = false;
	}
	return _localTransformation;
}

const glm::mat4 Link::getGlobalTransform() const
{
	return _globalTransformation;
}


const glm::mat4 &Link::getGlobalTransform()
{
	return _globalTransformation;
}


Link::Link()
	:_scene(nullptr)
	, _entityPtr(nullptr)
{
	reset();
}

Link::Link(EntityData *entity, AScene *scene)
{
	_entityPtr = entity;
	_scene = scene;
	reset();
#ifdef AGE_BFC
	initBFC(scene->getBfcBlockManagerFactory(), scene->getBfcLinkTracker());
#endif
}

void Link::reset()
{
	_position = glm::vec3(0);
	_scale = glm::vec3(1);
	_orientation = glm::quat(glm::mat4(1));
	_localTransformation = glm::mat4(1);
	_globalTransformation = glm::mat4(1);
	_localDirty = true;
	_parent = nullptr;
}

void Link::attachChild(Link *child)
{
	if (child->hasParent(this))
	{
		// already linked
		return;
	}

	if (child->_parent != nullptr)
	{
		// detach from previous parent
		child->_parent->_removeChild(child);
	}
	child->_setParent(this);
	_setChild(child);
	child->_updateGlobalTransform();
}

void Link::detachChild(Link *child)
{
	if (!child->hasParent(this))
	{
		// are not connected
		return;
	}
	child->_removeParent();
	child->_updateGlobalTransform();
	_removeChild(child);
}

void Link::detachChildren()
{
	for (auto &e : _children)
	{
		e->_removeParent();
		e->_updateGlobalTransform();
	}
	_children.clear();
	if (hasParent())
	{
		_detachFromRoot();
	}
}

bool Link::hasChild(const Link *child) const
{
	return child->hasParent(this);
}

void Link::attachParent(Link *parent)
{
	if (hasParent(parent))
	{
		// already linked
		return;
	}

	if (_parent != nullptr)
	{
		// detach from previous parent
		_parent->_removeChild(this);
	}
	_setParent(parent);
	_parent->_setChild(this);
	_parent->_updateGlobalTransform();
}

void Link::detachParent()
{
	if (!hasParent())
	{
		// are not connected
		return;
	}
	_parent->_removeChild(this);
	_removeParent();
	_updateGlobalTransform();
}

void Link::_setChild(Link *ptr)
{
	if (!hasParent())
	{
		_attachToRoot();
	}
	_children.push_back(ptr);
}

void Link::_setParent(Link *ptr)
{
	_detachFromRoot();
	_parent = ptr;
}

void Link::_removeChild(Link *ptr)
{
	if (_children.size() == 0)
		return;
	auto lastIndex = _children.size() - 1;
	for (std::size_t i = 0; i <= lastIndex; ++i)
	{
		if (_children[i] == ptr)
		{
			if (i != lastIndex)
			{
				std::swap(_children[i], _children[lastIndex]);
			}
			_children.resize(lastIndex);
			break;
		}
	}
	if (hasChildren() == false)
		_detachFromRoot();
}

void Link::_detachFromRoot()
{
	if (!_scene || !_parent)
		return; // because it's root
	auto root = _scene->getRootLink();
	if (_parent == root)
	{
		root->_removeChild(this);
		_removeParent();
	}
}

void Link::_attachToRoot()
{
	if (!_scene)
		return; // because it's root
	if (!hasParent())
	{
		auto root = _scene->getRootLink();
		root->_setChild(this);
	}
}

void Link::_removeParent()
{
	_detachFromRoot();
	_parent = nullptr;
}

void Link::_updateGlobalTransform()
{
	BFC_ADD();
	glm::mat4 p = glm::mat4(1);
	if (hasParent())
	{
		p = _parent->getGlobalTransform();
	}
	_globalTransformation = p * getLocalTransform();
	for (auto &e : _children)
	{
		e->_updateGlobalTransform();
	}
}
