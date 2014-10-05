#include "Link.hpp"
#include <Core/PrepareRenderThread.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

using namespace AGE;

void Link::registerOctreeObject(const PrepareKey &key)
{
	for (auto &b : _octreeObjects)
	{
		if (b.invalid())
		{
			b = key;
			auto ot = static_cast<PrepareRenderThread*>(_octree);
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
	auto ot = static_cast<PrepareRenderThread*>(_octree);
	for(auto &e : _octreeObjects)
	{
		if (e.invalid())
			return;
		ot->setPosition(_position, e);
	}
}

void Link::setForward(const glm::vec3 &v)
{
	_computeTrans = true;
	glm::vec4 get = glm::mat4(glm::toMat4(_orientation) * glm::translate(glm::mat4(1), v))[3];
	_position.x = _position.x + get.x;
	_position.y = _position.y + get.y;
	_position.z = _position.z + get.z;
	auto ot = static_cast<PrepareRenderThread*>(_octree);
	for (auto &e : _octreeObjects)
	{
		if (e.invalid())
			return;
		ot->setPosition(_position, e);
	}
}

void Link::setScale(const glm::vec3 &v) {
	_computeTrans = true;
	_scale = v;
	auto ot = static_cast<PrepareRenderThread*>(_octree);
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
	auto ot = static_cast<PrepareRenderThread*>(_octree);
	for (auto &e : _octreeObjects)
	{
		if (e.invalid())
			return;
		ot->setOrientation(_orientation, e);
	}
}

void Link::unregisterOctreeObject(const PrepareKey &key)
{
	for (auto &b : _octreeObjects)
	{
		if (b == key)
		{
			b = PrepareKey();
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
			_octreeObjects.fill(PrepareKey());
		}
