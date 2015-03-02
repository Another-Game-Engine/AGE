#include "Link.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Threads/RenderScene.hpp>
#include <Utils/Debug.hpp>
#include <Utils/MatrixConversion.hpp>

using namespace AGE;

void Link::registerOctreeObject(const PrepareKey &key)
{
	_octreeObjects.resize(_octreeObjects.size() + 1);
	auto &b = _octreeObjects.back();
	AGE_ASSERT(b.invalid());

	b = key;
	//_renderScene->setPosition(_position, key);
	//_renderScene->setScale(_scale, key);
	//_renderScene->setOrientation(_orientation, key);
	_renderScene->setTransform(getGlobalTransform(), key);
}

void Link::unregisterOctreeObject(const PrepareKey &key)
{
	for (std::size_t i = 0; i < _octreeObjects.size(); ++i)
	{
		auto &b = _octreeObjects[i];
		if (b == key)
		{
			_renderScene->removeElement(b);
			if (i != _octreeObjects.size() - 1)
			{
				std::swap(b, _octreeObjects.back());
			}
			_octreeObjects.pop_back();
			return;
		}
	}
}

void Link::setPosition(const glm::vec3 &v)
{
	_userModification = true;
	internalSetPosition(v);
}

void Link::setScale(const glm::vec3 &v) {
	_userModification = true;
	internalSetScale(v);
}

void Link::setOrientation(const glm::quat &v) {
	_userModification = true;
	internalSetOrientation(v);
}

void Link::setForward(const glm::vec3 &v)
{
	_userModification = true;
	internalSetForward(v);
}

void Link::setTransform(const glm::mat4 &t)
{
	_userModification = true;
	internalSetTransform(t);

}


void Link::internalSetPosition(const glm::vec3 &v)
{
	_localDirty = true;
	_position = v;

	_updateGlobalTransform();
}

void Link::internalSetForward(const glm::vec3 &v)
{
	_localDirty = true;
	glm::vec4 get = glm::mat4(glm::toMat4(_orientation) * glm::translate(glm::mat4(1), v))[3];
	_position.x = _position.x + get.x;
	_position.y = _position.y + get.y;
	_position.z = _position.z + get.z;

	_updateGlobalTransform();
}

void Link::internalSetTransform(const glm::mat4 &t)
{
	//_localDirty = true;
	//auto p = t * glm::vec4(1,1,1,1);
	//_position = glm::vec3(p.x, p.y, p.z);
	//_scale = scaleFromMat4(t);
	//_localTransformation = p;
	//auto rot = rotFromMat4(t, true);
	//_orientation = glm::quat(rot);
	//_position = posFromMat4(t);
	_localTransformation = glm::translate(t, _position);

	_updateGlobalTransform();
}


void Link::internalSetScale(const glm::vec3 &v) {
	_localDirty = true;
	_scale = v;

	_updateGlobalTransform();
}

void Link::internalSetOrientation(const glm::quat &v) {
	_localDirty = true;
	_orientation = v;

	_updateGlobalTransform();
}

const glm::mat4 Link::getLocalTransform() const
{
	if (_localDirty)
	{
		auto trans = glm::mat4(1);
		trans = glm::translate(_localTransformation, _position);
		trans = _localTransformation * glm::toMat4(_orientation);
		trans = glm::scale(_localTransformation, _scale);
		return trans;
	}
	return _localTransformation;
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
{
	reset();
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
	if (hasParent())
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
	if (!_renderScene || !_parent)
		return; // because it's root
	auto root = _renderScene->getRootLink();
	if (_parent == root)
	{
		root->_removeChild(this);
		_removeParent();
	}
}

void Link::_attachToRoot()
{
	if (!_renderScene)
		return; // because it's root
	if (!hasParent())
	{
		auto root = _renderScene->getRootLink();

		root->_children.push_back(this);
		this->_parent = root;
	}
}

void Link::_removeParent()
{
	_detachFromRoot();
	_parent = nullptr;
}

void Link::_updateGlobalTransform()
{
	glm::mat4 p = glm::mat4(1);
	if (hasParent())
	{
		p = _parent->getGlobalTransform();
	}
	_globalTransformation = p * getLocalTransform();
	for (auto &e : _octreeObjects)
	{
		AGE_ASSERT(!e.invalid());
		_renderScene->setTransform(_globalTransformation, e);
	}
	for (auto &e : _children)
	{
		e->_updateGlobalTransform(); 
	}
}
