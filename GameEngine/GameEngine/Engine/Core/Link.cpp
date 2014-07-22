#include "Link.hpp"
#include <Core/Octree.hpp>

using namespace AGE;

void Link::registerOctreeObject(const OctreeKey &key)
{
	for (auto &b : _octreeObjects)
	{
		if (b.invalid())
		{
			b = key;
			auto ot = static_cast<Octree*>(_octree);
			ot->setPosition(_position, key);
			ot->setScale(_scale, key);
			ot->setOrientation(_orientation, key);
			return;
		}
	}
	assert(false);
}

void Link::setPosition(const glm::vec3 &v)
{
	_computeTrans = true;
	_position = v;
	auto ot = static_cast<Octree*>(_octree);
	for(auto &e : _octreeObjects)
	{
		if (e.invalid())
			return;
		ot->setPosition(_position, e);
	}
}

void Link::setScale(const glm::vec3 &v) {
	_computeTrans = true;
	_scale = v;
	auto ot = static_cast<Octree*>(_octree);
	for (auto &e : _octreeObjects)
	{
		if (e.invalid())
			return;
		ot->setScale(_scale, e);
	}
}
void Link::setOrientation(const glm::quat &v) {
	_computeTrans = true;
	_orientation = v;
	auto ot = static_cast<Octree*>(_octree);
	for (auto &e : _octreeObjects)
	{
		if (e.invalid())
			return;
		ot->setOrientation(_orientation, e);
	}
}

void Link::unregisterOctreeObject(const OctreeKey &key)
{
	for (auto &b : _octreeObjects)
	{
		if (b == key)
		{
			b = OctreeKey();
			return;
		}
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
			_octreeObjects.fill(OctreeKey());
		}
