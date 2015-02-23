#include "Link.hpp"
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <Threads/RenderScene.hpp>
#include <Utils/Debug.hpp>

using namespace AGE;

void Link::registerOctreeObject(const PrepareKey &key)
{
	_octreeObjects.resize(_octreeObjects.size() + 1);
	auto &b = _octreeObjects.back();
	AGE_ASSERT(b.invalid());

	b = key;
	_renderScene->setPosition(_position, key);
	_renderScene->setScale(_scale, key);
	_renderScene->setOrientation(_orientation, key);
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

void Link::internalSetPosition(const glm::vec3 &v)
{
	_computeTrans = true;
	_position = v;
	auto ot = static_cast<RenderScene*>(_renderScene);
	for (auto &e : _octreeObjects)
	{
		AGE_ASSERT(!e.invalid());
		ot->setPosition(_position, e);
	}
}

void Link::internalSetForward(const glm::vec3 &v)
{
	_computeTrans = true;
	glm::vec4 get = glm::mat4(glm::toMat4(_orientation) * glm::translate(glm::mat4(1), v))[3];
	_position.x = _position.x + get.x;
	_position.y = _position.y + get.y;
	_position.z = _position.z + get.z;

	for (auto &e : _octreeObjects)
	{
		AGE_ASSERT(!e.invalid());
		_renderScene->setPosition(_position, e);
	}
}

void Link::internalSetScale(const glm::vec3 &v) {
	_computeTrans = true;
	_scale = v;

	for (auto &e : _octreeObjects)
	{
		AGE_ASSERT(!e.invalid());
		_renderScene->setScale(_scale, e);
	}
}

void Link::internalSetOrientation(const glm::quat &v) {
	_computeTrans = true;
	_orientation = v;

	for (auto &e : _octreeObjects)
	{
		AGE_ASSERT(!e.invalid());
		_renderScene->setOrientation(_orientation, e);
	}
}

const glm::mat4 &Link::getTransform()
{
	if (_computeTrans)
	{
		_trans = glm::mat4(1);
		_trans = glm::translate(_trans, _position);
		_trans = _trans * glm::toMat4(_orientation);
		_trans = glm::scale(_trans, _scale);
		_computeTrans = false;
	}
	return _trans;
}

const glm::mat4 &Link::getTransform() const
{
	if (_computeTrans)
	{
		auto trans = glm::mat4(1);
		trans = glm::translate(_trans, _position);
		trans = _trans * glm::toMat4(_orientation);
		trans = glm::scale(_trans, _scale);
		return trans;
	}
	return _trans;
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
	_trans = glm::mat4(1);
	_computeTrans = true;
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
}

void Link::detachChild(Link *child)
{
	if (!child->hasParent(this))
	{
		// are not connected
		return;
	}
	child->_removeParent();
	_removeChild(child);
}

void Link::detachChildren()
{
	for (auto &e : _children)
	{
		e->_removeParent();
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
	_setParent(_parent);
	_parent->_setChild(this);
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
